#ifndef __INSTR_XOR_H__
#define __INSTR_XOR_H__
/*
Put the declarations of `xor' instructions here.
*/
make_instr_func(xor_r2rm_v);//0x01
make_instr_func(xor_r2rm_b);//0x00
make_instr_func(xor_rm2r_b);//0x02
make_instr_func(xor_rm2r_v);//0x03
make_instr_func(xor_i2a_b); //0x04
make_instr_func(xor_i2a_v); //0x05

make_instr_func(xor_i2rm_b);
make_instr_func(xor_i2rm_v);
make_instr_func(xor_i2rm_bv);

#endif
