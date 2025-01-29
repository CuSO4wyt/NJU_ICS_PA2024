#include "cpu/instr.h"

static void instr_execute_1op() 
{
    operand_read(&opr_src);
    
    opr_src.val = ~opr_src.val;
    
    uint32_t mask = 0xffffffff >> (32 - data_size);
    
    opr_src.val = opr_src.val & mask;
    
    operand_write(&opr_src);
}

make_instr_impl_1op(not, rm, v)