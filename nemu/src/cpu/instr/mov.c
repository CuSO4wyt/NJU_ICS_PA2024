#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, rm, r, w)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}


// 获取控制寄存器的引用
#define GET_CR(op) ((op) == 0 ? &cpu.cr0.val : (op) == 3 ? &cpu.cr3.val : NULL)


make_instr_func(mov_c2r_l) 
{
    int len = 1;
    OPERAND cr, rm;
    uint8_t op;

    rm.data_size = data_size;
    len += modrm_opcode_rm(eip + 1, &op, &rm);

    // 获取控制寄存器值
    uint32_t *cr_val = GET_CR(op);
    assert(cr_val != NULL); // 检查是否是有效的控制寄存器

    rm.val = *cr_val; // 从控制寄存器加载值
    operand_write(&rm); // 写入目标通用寄存器

    print_asm_2("mov", "c2r", len, &cr, &rm);
    return len;
}

make_instr_func(mov_r2c_l) 
{
    int len = 1;
    OPERAND rm, cr;
    uint8_t op;

    rm.data_size = data_size;
    len += modrm_opcode_rm(eip + 1, &op, &rm);

    // 获取控制寄存器值
    uint32_t *cr_val = GET_CR(op);
    assert(cr_val != NULL); // 检查是否是有效的控制寄存器

    operand_read(&rm); // 从源通用寄存器读取值
    *cr_val = rm.val;  // 写入控制寄存器

    print_asm_2("mov", "r2c", len, &rm, &cr);
    return len;
}
