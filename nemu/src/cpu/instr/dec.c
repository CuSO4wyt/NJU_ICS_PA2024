#include "cpu/instr.h"
/*
Put the implementations of `dec' instructions here.
*/

static void instr_execute_1op()
{
    operand_read(&opr_src);
    
    bool temp = cpu.eflags.CF;
    
    opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
    
    uint32_t temp_sub = sign_ext((uint32_t)1, opr_src.data_size);
    
    opr_src.val = alu_sub(temp_sub,opr_src.val,data_size);
    
    cpu.eflags.CF = temp;
    
    assert(cpu.eflags.CF == temp);
    
    operand_write(&opr_src);
}

make_instr_impl_1op(dec, r, v)
make_instr_impl_1op(dec, rm, v)