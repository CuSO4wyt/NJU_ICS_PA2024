#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/
make_instr_func(lgdt) {
    OPERAND m;
    int len = 1;

    // 初始化操作数属性
    m.data_size = 16;
    len += modrm_rm(eip + 1, &m);

    // 读取 GDTR 限长字段（16 位）
    operand_read(&m);
    cpu.gdtr.limit = m.val & 0xFFFF; // 确保只取低 16 位

    // 读取 GDTR 基地址字段（32 位）
    m.data_size = 32;
    m.addr += 2; // 偏移 2 字节以读取基地址
    operand_read(&m);
    cpu.gdtr.base = m.val;

    return len; // 返回指令长度
}