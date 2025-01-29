#include "cpu/instr.h"

make_instr_func(iret) 
{
    // 从栈中读取并更新 EIP、CS 和 EFLAGS
    cpu.eip = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;
    
    cpu.cs.val = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;
    
    
   
    //load sreg
    
    SegDesc seg;
    uint32_t index = cpu.segReg[SREG_CS].index;
    
    seg.val[0] = laddr_read(cpu.gdtr.base + index * 8, 4);
    seg.val[1] = laddr_read(cpu.gdtr.base + index * 8 + 4, 4);
    
    uint32_t base = (seg.base_31_24 << 24) | (seg.base_23_16 << 16) | (seg.base_15_0);

    
    cpu.segReg[SREG_CS].base = base;
    
    uint32_t limit = seg.limit_15_0 | (seg.limit_19_16 << 16);
    
  
    
    cpu.segReg[SREG_CS].limit = limit;
    
    
    cpu.segReg[SREG_CS].soft_use = seg.soft_use;
    cpu.segReg[SREG_CS].type = seg.type;
    cpu.segReg[SREG_CS].privilege_level = seg.privilege_level;


    cpu.eflags.val = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;

    return 0;
}
