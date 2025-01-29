#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/
#include "cpu/instr.h"

make_instr_func(leave_)
{
    // 将 ESP 设置为 EBP 的值
    cpu.esp = cpu.ebp;

    // 从栈中弹出旧的 EBP
    uint32_t old_ebp = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.ebp = old_ebp;

    // 更新 ESP，指向下一个栈位置
    cpu.esp += data_size / 8;

    // 返回指令长度为 1 字节
    return 1;
}
