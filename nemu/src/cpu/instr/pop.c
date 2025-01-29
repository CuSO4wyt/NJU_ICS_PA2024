#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/



static void instr_execute_1op()
{

    // 读取堆栈顶的值 
    opr_src.val = vaddr_read(cpu.esp, SREG_SS, data_size/8);
    
    // 更新堆栈指针 ESP，指向下一个堆栈项

    cpu.esp += data_size/8;
    
    // 将读取的值写入目标操作数 (寄存器或内存)
    operand_write(&opr_src);
    
    
}

make_instr_func(pop_a) 
{
    // 从栈中依次弹出值，并赋值给对应的寄存器
    cpu.edi = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;

    cpu.esi = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;

    cpu.ebp = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;

    // 跳过 ESP
    cpu.esp += 4;

    cpu.ebx = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;

    cpu.edx = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;

    cpu.ecx = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;

    cpu.eax = vaddr_read(cpu.esp, SREG_SS, 4);
    cpu.esp += 4;

    print_asm_0("popa", "", 1);

    return 1;
}


make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop,rm,v)