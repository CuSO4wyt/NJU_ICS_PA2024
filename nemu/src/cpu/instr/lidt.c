#include "cpu/instr.h"
/*
Put the implementations of `lidt' instructions here.
*/

make_instr_func(lidt) 
{
#ifdef  IA32_INTR 

    OPERAND m;
    // 解析 ModR/M 字节，获取操作数地址
    m.data_size = 16;
    
    int len = 1;
    len += modrm_rm(eip + 1, &m);

    // 读取 IDT 限长字段
    operand_read(&m);
    cpu.idtr.limit = m.val & 0xFFFF; // 确保只取低 16 位

    // 读取 IDT 基地址字段
    m.addr += 2;      // 跳过限长字段
    m.data_size = 32; // 基地址字段大小为 32 位
    operand_read(&m);
    cpu.idtr.base = m.val;

    return len; // 返回指令长度
#else
    return 0;
    
#endif
}
