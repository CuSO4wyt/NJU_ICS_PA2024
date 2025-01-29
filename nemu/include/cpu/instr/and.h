#ifndef __INSTR_AND_H__
#define __INSTR_AND_H__
/*
Put the declarations of `and' instructions here.
*/
make_instr_func(and_r2rm_v);
make_instr_func(and_r2rm_b);//0x00
make_instr_func(and_rm2r_b);//0x02
make_instr_func(and_rm2r_v);//0x03
make_instr_func(and_i2a_b); //0x04
make_instr_func(and_i2a_v); //0x05


make_instr_func(and_i2rm_b);//80
make_instr_func(and_i2rm_v);//81
make_instr_func(and_i2rm_bv);
#endif
