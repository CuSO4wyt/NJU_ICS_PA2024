#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
make_instr_func(call_near)
{
    OPERAND rel;

    // 初始化操作数以读取立即数
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = cpu.eip + 1;

    // 从指令中读取立即数
    operand_read(&rel);

    // 计算指令长度
    int len = 1 + data_size / 8;

    // 对立即数进行符号扩展，处理负偏移
    int32_t offset = sign_ext(rel.val, data_size);

    // 计算返回地址（call指令后的地址）
    uint32_t return_addr = cpu.eip + len;

    // 将返回地址压入栈
    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, return_addr);

    // 更新指令指针到目标地址
    cpu.eip += offset + len;

    return 0;
}

make_instr_func(call_indirect)
{
    OPERAND rm;

    // 初始化操作数以读取 ModR/M 字节
    rm.data_size = data_size;
    int len = modrm_rm(cpu.eip + 1, &rm);

    // 从操作数中读取目标地址
    operand_read(&rm);

    // 计算返回地址（call 指令后的地址）
    uint32_t return_addr = cpu.eip + 1 + len;

    // 将返回地址压入栈
    cpu.esp -= 4;
    vaddr_write(cpu.esp, SREG_SS, 4, return_addr);

    // 更新指令指针到目标地址
    cpu.eip = rm.val;

    return 0;
}