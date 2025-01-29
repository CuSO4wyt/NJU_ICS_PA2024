#include "cpu/instr.h"
/*
Put the implementations of `add' instructions here.
*/

static void instr_execute_2op()
{
    operand_read(&opr_src);
    operand_read(&opr_dest);
    
    opr_src.val = sign_ext(opr_src.val,opr_src.data_size);
    opr_dest.val = sign_ext(opr_dest.val,opr_dest.data_size);
    
    opr_dest.val = alu_add(opr_src.val,opr_dest.val, data_size);
    
    operand_write(&opr_dest);
}

make_instr_impl_2op(add,r,rm,v)//0x01
make_instr_impl_2op(add,r,rm,b)//0x00
make_instr_impl_2op(add,rm,r,b)//0x02
make_instr_impl_2op(add,rm,r,v)//0x03

make_instr_impl_2op(add,i,rm,b)//0x80
make_instr_impl_2op(add,i,rm,v)//0x81
make_instr_impl_2op(add,i,rm,bv)//0x82

make_instr_impl_2op(add,i,a,b)
make_instr_impl_2op(add,i,a,v)