#include "cpu/instr.h"
/*
Put the implementations of `or' instructions here.
*/

static void instr_execute_2op()
{
    operand_read(&opr_src);
    operand_read(&opr_dest);
    
    opr_src.val = sign_ext(opr_src.val,opr_src.data_size);
    opr_dest.val = sign_ext(opr_dest.val,opr_dest.data_size);

    opr_dest.val = alu_or(opr_src.val,opr_dest.val,data_size);
    
    cpu.eflags.CF = cpu.eflags.OF = 0;
    assert(cpu.eflags.CF == 0 && cpu.eflags.OF == 0);
    
    operand_write(&opr_dest);
}

make_instr_impl_2op(or,r,rm,b) //0x08
make_instr_impl_2op(or,r,rm,v) //0x09
make_instr_impl_2op(or,rm,r,b) //0x0a
make_instr_impl_2op(or,rm,r,v) //0x0b

make_instr_impl_2op(or,i,a,b)
make_instr_impl_2op(or,i,a,v)

make_instr_impl_2op(or,i,rm,b) //0x80
make_instr_impl_2op(or,i,rm,v) //0x81
make_instr_impl_2op(or,i,rm,bv) //0x83
