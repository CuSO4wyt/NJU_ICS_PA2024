#include "common.h"
#include "memory.h"
#include "string.h"

#include <elf.h>

#ifdef HAS_DEVICE_IDE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader()
{
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph, *eph;

#ifdef HAS_DEVICE_IDE 
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);  // 从硬盘读取 ELF 文件头
	elf = (void *)buf;
	Log("ELF loading from hard disk.");
#else
	elf = (void *)0x0;
	Log("ELF loading from ram disk.");
#endif

	/* Load each program segment */
	ph = (void *)elf + elf->e_phoff;  // 找到 ELF 文件中的程序头表
	eph = ph + elf->e_phnum;
	for (; ph < eph; ph++)
	{
		if (ph->p_type == PT_LOAD)  // 如果是 LOAD 段，则进行加载
		{
#ifndef IA32_PAGE
            void* addr = (void*)ph->p_vaddr;  // 非分页模式，直接使用虚拟地址
#else
			void* addr = (void*)mm_malloc(ph->p_vaddr, ph->p_memsz);  // 分页模式，分配内存
#endif

#ifdef HAS_DEVICE_IDE
			ide_read(addr, ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);  // 从硬盘读取段数据
#else
			memcpy(addr, (void *)ph->p_offset, ph->p_filesz);  // 从内存中复制段数据
#endif

			memset((void *)(addr + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);  // 清零未使用的内存区域

#ifdef IA32_PAGE
			/* Record the program break for future use */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if (brk < new_brk)
			{
				brk = new_brk;
			}
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;  // 获取 ELF 文件的入口地址

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);  // 分配栈空间
#ifdef HAS_DEVICE_VGA
	create_video_mapping();  // 创建显存映射
#endif
	write_cr3(get_ucr3());  // 设置 CR3 寄存器
#endif
	return entry;  // 返回 ELF 文件的入口地址
}