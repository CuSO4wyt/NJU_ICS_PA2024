#include "cpu/instr.h"
/*
Put the implementations of `inc' instructions here.
*/

static void instr_execute_1op()
{
    operand_read(&opr_src);
    
    bool temp = cpu.eflags.CF;
    
    opr_src.val =  sign_ext(opr_src.val, opr_src.data_size);
    
    uint32_t temp_add = sign_ext((uint32_t)1, opr_src.data_size);
    
    opr_src.val = alu_add(temp_add, opr_src.val, data_size);
    
    cpu.eflags.CF = temp;
    
    operand_write(&opr_src);
}

make_instr_impl_1op(inc, r, v)
make_instr_impl_1op(inc, rm, v)

