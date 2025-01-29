#ifndef __OPERAND_H__
#define __OPERAND_H__

#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

// operand type for immediate number, register, and memory
enum
{
	OPR_IMM, //立即数
	OPR_REG, //寄存器
	OPR_MEM, //一个内存地址
	OPR_CREG,//控制寄存器编号
	OPR_SREG //段寄存器编号
};

#define MEM_ADDR_NA 0xffffffff

//enum {MEM_ADDR_OFF, MEM_ADDR_SIB};

typedef struct
{
	//	uint32_t type;
	uint32_t disp;  // hex
	uint32_t base;  // register
	uint32_t index; // register
	uint32_t scale; // 1, 2, 4, 8
} MEM_ADDR;			// memory address details

typedef struct
{
	int type;
	uint32_t addr;
	uint8_t sreg;
	uint32_t val;
	size_t data_size;
	MEM_ADDR mem_addr;
} OPERAND;

extern OPERAND opr_src, opr_dest;

// read the operand's value from its addr
void operand_read(OPERAND *opr);

// write the operand's value to its addr
void operand_write(OPERAND *opr);
void operand_write_cr0(OPERAND *opr);

void parse_operand_address(OPERAND *opr, char *str);
void clear_operand_mem_addr(OPERAND *opr);

#endif
