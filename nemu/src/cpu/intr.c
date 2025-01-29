#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

#include "cpu/cpu.h"
#include "memory/memory.h"

void raise_intr(uint8_t intr_no) 
{
#ifdef IA32_INTR

    //  保存现有上下文：EFLAGS, CS, EIP
    cpu.esp -= 4; 
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.eflags.val);

    cpu.esp -= 4; 
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.cs.val);

    cpu.esp -= 4; 
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.eip);

    //  从 IDT 中获取对应的门描述符
    GateDesc gate_entry;
    uint32_t desc_addr = cpu.idtr.base + (intr_no << 3);
    
    gate_entry.val[0] = laddr_read(desc_addr, 4);
    gate_entry.val[1] = laddr_read(desc_addr + 4, 4);

   
    //  设置新的段选择子，并加载段寄存器
    cpu.cs.val = gate_entry.selector;
    
    load_sreg(SREG_CS);

     if (gate_entry.type == 0xe) 
    {
        cpu.eflags.IF = 0;
    }


    uint32_t handler_offset = (gate_entry.offset_31_16 << 16) + gate_entry.offset_15_0;
    
    uint32_t addr = segment_translate(handler_offset, SREG_CS);
    
    cpu.eip = addr;
#endif
}


void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
