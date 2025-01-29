#include "cpu/instr.h"
#include "device/port_io.h"


static  bool is_valid_data_size(uint32_t data_size)
{
    return (data_size == 8 || data_size == 16 || data_size == 32);
}


static  bool is_valid_port(uint16_t port)
{
    return (port <= 0xFFFF); 
}


static void in_common(OPERAND *ra, OPERAND *rd, int data_size)
{
    // 从端口读取数据，按字节读取
    if(!is_valid_data_size(data_size)|| !is_valid_port(rd->val))
    {
        assert(0);
    }
    
    
    uint32_t data = pio_read(rd->val, data_size / 8);

    
    if (data_size == 8)
    {
        ra->val = (uint8_t)data;
    }
    else if (data_size == 16)
    {
        ra->val = (uint16_t)data;
    } 
    else
    {
        ra->val = data;
    }

    operand_write(ra); 
}


make_instr_func(in_b)
{
    OPERAND ra, rd;

    // 设置目标寄存器 EAX，数据大小为 8 位
    ra.data_size = 8;
    rd.data_size = 16;
    
    ra.type = rd.type = OPR_REG;
    ra.addr = 0x0;
    rd.addr = 0x2;

    operand_read(&rd);

  
    in_common(&ra, &rd, 8);

    return 1;  // 处理一个字节
}


make_instr_func(in_v)
{
    OPERAND ra, rd;

    // 设置目标寄存器 EAX，数据大小由 data_size 指定
    ra.data_size = data_size;
    rd.data_size = 16;
    
    ra.type = rd.type = OPR_REG;
    
    ra.addr = 0x0;
    rd.addr = 0x2;

    // 读取 EDX 中的端口地址
    operand_read(&rd);

   
    in_common(&ra, &rd, data_size);


    return 1;  // 处理一个数据单元（字节、字或双字）
}
