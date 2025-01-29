#include "cpu/instr.h"
/*
Put the implementations of `xor' instructions here.
*/


static void instr_execute_2op()
{
    operand_read(&opr_src);
    operand_read(&opr_dest);
    
    opr_src.val = sign_ext(opr_src.val, opr_src.data_size);
    opr_dest.val = sign_ext(opr_dest.val,opr_dest.data_size);
 
    opr_dest.val = alu_sub(opr_src.val,opr_dest.val,data_size);
    
    
    operand_write(&opr_dest);
    
}


make_instr_impl_2op(xor,r,rm,v)//0x01
make_instr_impl_2op(xor,r,rm,b)//0x00
make_instr_impl_2op(xor,rm,r,b)//0x02
make_instr_impl_2op(xor,rm,r,v)//0x03
make_instr_impl_2op(xor,i,a,b)
make_instr_impl_2op(xor,i,a,v)
make_instr_impl_2op(xor,i,rm,b)//80
make_instr_impl_2op(xor,i,rm,v)//81
make_instr_impl_2op(xor,i,rm,bv)