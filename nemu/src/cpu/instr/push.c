#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/
//push register 都是32bit

static void instr_execute_1op()
{
    operand_read(&opr_src);
    
    cpu.esp -= data_size/8;
    
    vaddr_write(cpu.esp, SREG_SS, data_size/8 , opr_src.val);
}

make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm,v)
make_instr_impl_1op(push, i, v)
make_instr_impl_1op(push, i, b)

make_instr_func(push_a) 
{
    //!!注意顺序！！依次压栈
    int len = 1;
    uint32_t temp = cpu.esp;

    
    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.eax);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.ecx);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.edx);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.ebx);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, temp);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.ebp);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.esi);

    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, cpu.edi);
    
    print_asm_0("pusha", "", 1);

    return len;
}
