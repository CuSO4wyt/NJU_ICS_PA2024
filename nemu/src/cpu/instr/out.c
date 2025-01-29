#include "cpu/instr.h"
#include "device/port_io.h"
#include "cpu/reg.h"
#include <stdio.h>


static  bool is_valid_data_size(uint32_t data_size)
{
    return (data_size == 8 || data_size == 16 || data_size == 32);
}


static  bool is_valid_port(uint16_t port)
{
    return (port <= 0xFFFF); 
}

make_instr_func(out_b)
{
    // 定义源操作数（AL）和目标操作数（DX）
    OPERAND src, dst;

    // 初始化源操作数（AL）
    src.data_size = 8;  // 8 位数据
    dst.data_size = 16; // 16 位数据
    
    src.type = dst.type = OPR_REG; // 寄存器类型
    
    src.addr = 0x0;     // AL 的寄存器编号
    dst.addr = 0x2;     // DX 的寄存器编号

    // 读取源操作数和目标操作数的值
    operand_read(&src);
    operand_read(&dst);

    // 获取要写入的数据和端口号
    uint8_t data = (uint8_t)src.val; // 取低 8 位
    uint16_t port = (uint16_t)dst.val; // 取低 16 位

    // 检查端口号是否合法
    if (!is_valid_port(port))
    {
        assert(0);
        return 0; // 返回 0 表示失败
    }

    // 写入数据到端口
    pio_write(port, 1, data);

    return 1; // 返回 1 表示成功
}

make_instr_func(out_v)
{
    // 定义源操作数（AX/EAX）和目标操作数（DX）
    OPERAND src, dst;

    src.data_size = data_size; // 数据大小由调用者指定
    src.type = dst.type = OPR_REG;   
    // 寄存器类型
    src.addr = 0x0;            // AX/EAX 的寄存器编号
    dst.addr = 0x2;            // DX 的寄存器编号
   
   
    dst.data_size = 16;        


    // 读取源操作数和目标操作数的值
    operand_read(&src);
    operand_read(&dst);

    // 获取要写入的数据和端口号
    uint32_t data = src.val;   // 数据
    uint16_t port = (uint16_t)dst.val; // 端口号

    // 检查数据大小是否合法
    if (!is_valid_data_size(data_size))
    {
        assert(0);
        return 0; // 返回 0 表示失败
    }

    // 检查端口号是否合法
    if (!is_valid_port(port))
    {
        assert(0);
        return 0; // 返回 0 表示失败
    }

    // 根据数据大小写入数据
    switch (data_size)
    {
        case 8:
            pio_write(port, 1, (uint8_t)data); // 写入 8 位数据
            break;
        case 16:
            pio_write(port, 2, (uint16_t)data); // 写入 16 位数据
            break;
        case 32:
            pio_write(port, 4, data); // 写入 32 位数据
            break;
        default:
            assert(0);
            return 0; // 返回 0 表示失败
    }

    return 1; // 返回 1 表示成功
}