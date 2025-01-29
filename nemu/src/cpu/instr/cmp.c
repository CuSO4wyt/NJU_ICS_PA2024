#include "cpu/instr.h"

static void instr_execute_2op()
{
    operand_read(&opr_src);
    operand_read(&opr_dest);
    
    uint32_t temp_src = sign_ext(opr_src.val, opr_src.data_size);
    uint32_t temp_dest = sign_ext(opr_dest.val, opr_dest.data_size);
    
    alu_sub(temp_src,temp_dest,data_size);
    
    operand_write(&opr_dest);
}

make_instr_impl_2op(cmp, r, rm, b)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, rm, r, v)
make_instr_impl_2op(cmp, i, a, b)
make_instr_impl_2op(cmp, i, a, v)
make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, i, rm, bv)