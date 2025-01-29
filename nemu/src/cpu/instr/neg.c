#include "cpu/instr.h"


static void instr_execute_1op()
{
    operand_read(&opr_src);
    
    // 设置 CF 标志
    bool temp = (opr_src.val == 0) ? 0 : 1;
    
    opr_src.val = (-opr_src.val);
    
    cpu.eflags.CF = temp;
    
    operand_write(&opr_src);
}

make_instr_impl_1op(neg, rm, v)
make_instr_impl_1op(neg, rm, b)