#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}



void hw_mem_write(paddr_t paddr, size_t len, uint32_t data)
{
	memcpy(hw_mem + paddr, &data, len);
}




uint32_t paddr_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;

#ifdef HAS_DEVICE_VGA
    int map_NO = is_mmio(paddr);
    if ((is_mmio(paddr)) != -1)
    {
        return mmio_read(paddr, len, map_NO);
    }
#endif



#ifdef CACHE_ENABLED
	ret = cache_read(paddr, len);     // 通过cache进行读
#else
	ret = hw_mem_read(paddr, len);
#endif


	return ret;

    
}



void paddr_write(paddr_t paddr, size_t len, uint32_t data)
{

#ifdef HAS_DEVICE_VGA

    int map_NO = is_mmio(paddr);
    if ((is_mmio(paddr)) != -1)
    {
        mmio_write(paddr, len, data, map_NO);
        return;
    }
#endif



#ifdef CACHE_ENABLED
	cache_write(paddr, len, data);    // 通过cache进行写
#else
	hw_mem_write(paddr, len, data);
#endif


}



uint32_t laddr_read(laddr_t laddr, size_t len)
{
#ifdef IA32_PAGE
    if(cpu.cr0.pg && cpu.cr0.pe)
    {
        uint32_t ret = 0;
        //是否跨页？
        // 如果没有跨页，直接读取
        if (((laddr + len - 1) >> 12) == (laddr >> 12)) 
        {
            // 对于每个字节，逐个转换为物理地址并读取
            for (uint32_t i = 0; i < len; i++) 
            {
                uint32_t laddr_i = page_translate(laddr + i);  
                ret |= paddr_read(laddr_i, 1) << (8 * i);  
            }
        } 
        else 
        {
            // 如果跨页，分别读取两个页
            size_t nextPage = (((laddr >> 12) + 1) << 12) - laddr;  
            paddr_t p1 = page_translate(laddr);  
            paddr_t p2 = page_translate(laddr + nextPage);  
            uint32_t res1 = paddr_read(p1, nextPage);  
            uint32_t res2 = paddr_read(p2, len - nextPage);  

            // 拼接两个物理页的数据
            ret = (res2 << (8 * nextPage)) | res1;
        }
        return ret;
    }

#endif
    return paddr_read(laddr, len);
}



void laddr_write(laddr_t laddr, size_t len, uint32_t data)
{
    
#ifdef IA32_PAGE
    if (cpu.cr0.pg == 1 && cpu.cr0.pe)
    {  // 如果启用了分页模式
        size_t page_offset = laddr & PAGE_MASK;
       
        // 如果不跨页
        if (page_offset + len <= PAGE_SIZE)
        {
            for (size_t i = 0; i < len; i++)
            {
                uint32_t laddr_i = page_translate(laddr + i);  // 获取每个字节的物理地址
                paddr_write(laddr_i, 1, (data >> (8 * i)) & 0xff);  // 写入每个字节
            }
        } 
        else
        {
            // 跨页的情况，需要分别处理
            size_t first_page_len = PAGE_SIZE - page_offset;  // 第一页的剩余长度
            size_t second_page_len = len - first_page_len;    // 第二页的长度

            // 获取第一和第二页的物理地址
            paddr_t p1 = page_translate(laddr);  // 第一页的物理地址
            paddr_t p2 = page_translate(laddr + first_page_len);  // 第二页的物理地址

            // 处理第一页
            paddr_write(p1, first_page_len, data & ((1 << (8 * first_page_len)) - 1));

            // 处理第二页
            paddr_write(p2, second_page_len, data >> (8 * first_page_len));
        }
        return;
    }
#endif
    paddr_write(laddr, len, data);  // 非分页模式直接写入
}



uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
#ifdef IA32_SEG
	if (cpu.cr0.pe == 1) 
	{
		vaddr = segment_translate(vaddr, sreg);
	}
#endif
	return laddr_read(vaddr, len);
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
#ifdef IA32_SEG
	if (cpu.cr0.pe == 1) 
	{
		vaddr = segment_translate(vaddr, sreg);
	}
#endif
	laddr_write(vaddr, len, data);
}

void init_mem()
{
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
#ifdef CACHE_ENABLED
	init_cache();
#endif
#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t *get_mem_addr()
{
	return hw_mem;
}