#include "cpu/instr.h"

static inline void handle_return(uint16_t imm_offset) 
{
    // 从栈顶读取返回地址
    uint32_t return_address = vaddr_read(cpu.esp, SREG_SS, data_size / 8);
    cpu.eip = return_address;

    // 更新 ESP：弹出返回地址并增加偏移量
    cpu.esp += (data_size / 8) + imm_offset;
}

make_instr_func(ret_near) 
{
    handle_return(0); // 无额外偏移量
    return 0;         // 指令长度为 0
}

make_instr_func(ret_near_iw) 
{
    // 读取立即数（16 位）作为额外偏移量
    OPERAND imm;
    imm.type = OPR_IMM;
    imm.data_size = 16;
    imm.addr = eip + 1; // 立即数位于 EIP + 1 的位置
    operand_read(&imm);

    handle_return(imm.val); // 使用立即数作为额外偏移量
    return 0;              // 指令长度为 0
}