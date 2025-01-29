#include "cpu/cpu.h"
#include "assert.h"

#define MASK ((uint32_t)(0xFFFFFFFF >> (32 - data_size)))      
//Mask

void set_ZF(uint32_t res, size_t data_size) //set ZF(zero)
{
    res = res & MASK;
    cpu.eflags.ZF = (res == 0)?1:0; //结果全0设置为1，结果非全0设置为0
}


void set_CF_add(uint32_t res,uint32_t src,size_t data_size)//set CF-add 
{
    res = sign_ext(res & MASK, data_size);
    src = sign_ext(src & MASK, data_size);
    cpu.eflags.CF = (res < src)?1 : 0;
}



void set_PF(uint32_t res) //设置PF（奇偶标志位）
{
    
    int cnt = 0;
    for(int i = 0;i < 8;i++)
    {
        if(res%2 == 1)
        {
            cnt++;
        }
        res >>= 1; 
    }
    
    cpu.eflags.PF = (cnt % 2 == 0)?1:0;
}


void set_OF_add(uint32_t res, uint32_t src, uint32_t dest, size_t data_size) //Overflow flag
{
    
    res = res & MASK;
    src = src & MASK;
    dest = dest & MASK;
    
    bool src_sign = (src >> (data_size - 1)) &1;
    bool dest_sign = (dest >> (data_size-1)) &1;
    bool result_sign = (res >>(data_size-1)) &1;
    
    //溢出条件
    cpu.eflags.OF = (!src_sign && !dest_sign && result_sign) || (src_sign && dest_sign && !result_sign);
}


void set_SF(uint32_t result, size_t data_size) //signature flag?
{
    
    uint32_t sign_bit = (result >> (data_size-1)) &1;
    
    cpu.eflags.SF = sign_bit;
}



//add
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	uint32_t res = 0;
	
	src = src & MASK;
	dest = dest & MASK;
	
	res = (src + dest) & MASK;
	
	set_PF(res);
	set_SF(res,data_size);
	set_CF_add(res,src,data_size);
	set_OF_add(res,src,dest,data_size);
	set_ZF(res,data_size);
	 return res;
#endif
}


void set_CF_adc(uint32_t res,uint32_t src,size_t data_size)
{
    res = sign_ext(res & MASK, data_size);
    src = sign_ext(src & MASK, data_size);
    if(res < src)
    {
        cpu.eflags.CF = 1;
    }
    else if(res == src)
    {
        cpu.eflags.CF = (cpu.eflags.CF == 1)?1:0;
    }
    else
    {
        cpu.eflags.CF = 0;
    }
}

//adc
uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else

	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	
	uint32_t carry = cpu.eflags.CF; //获取当前CF标志位
	
	uint32_t res = (dest + src + carry) & MASK; //计算加上CF的结果
	
	set_CF_adc(res, src, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);
    set_ZF(res, data_size);
    set_PF(res);
	
    return res;
    
    
#endif
}



void set_CF_sub(uint32_t res, uint32_t dest, size_t data_size)//set CF-sub
{
    
    res = res & MASK;
    dest = dest & MASK;
    cpu.eflags.CF = (res > dest)?1:0;
}

void set_OF_sub(uint32_t res, uint32_t src, uint32_t dest, size_t data_size)
{
    set_OF_add(res, ~src, dest,data_size);
}


//sub

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);

    src = src & MASK;
    dest = dest & MASK;
	uint32_t temp = (~src + 1) & MASK;

	uint32_t res = (temp + dest) & MASK;
	
	set_CF_sub(res, dest, data_size);
	set_OF_sub(res, src, dest, data_size);
	set_SF(res, data_size);
	set_ZF(res, data_size); 
	set_PF(res);   
	
	return res;
#endif
}

void set_CF_sbb(uint32_t res, uint32_t dest, size_t data_size)
{
    res = sign_ext(res & MASK, data_size);
    dest = sign_ext(dest & MASK, data_size);
    
    cpu.eflags.CF = (res >= dest)?1:0;
    
}

//sbb
uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	uint32_t carry = cpu.eflags.CF;
	
	uint32_t res = 0;
	
	if(carry)
	{
	    res = ((~src) + dest) & MASK;
	    set_PF(res);
	    set_ZF(res,data_size);
	    set_SF(res,data_size);
	    set_CF_sbb(res, dest, data_size);
	    set_OF_sub(res, src,dest, data_size);
	}
	else
	{
	  res = alu_sub(src, dest, data_size);
	}
	
	return res;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	src = src & MASK;
	dest = dest & MASK;
	
	
	uint32_t res = (src & dest) & MASK;
	
	cpu.eflags.CF = 0;  // CF 清 0
    cpu.eflags.OF = 0;  // OF 清 0

	
	set_ZF(res,data_size);
	set_SF(res,data_size);
	set_PF(res);
	
	return res;
	
#endif
}

//or
uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	src = src & MASK;
	
	dest = dest & MASK;
    uint32_t res = (src | dest) & MASK;

    cpu.eflags.CF = 0;  // CF 清 0
    cpu.eflags.OF = 0;  // OF 清 0

    set_ZF(res, data_size); 
    set_SF(res, data_size);  
    set_PF(res);           

    return res;
	
#endif
}


 //xor
uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	src = src & MASK;
	dest = dest & MASK;
	
	uint32_t res =(src ^ dest) & MASK;

    cpu.eflags.CF = 0;  // CF 清 0
    cpu.eflags.OF = 0;  // OF 清 0

    set_ZF(res, data_size); 
    set_SF(res, data_size);  
    set_PF(res); 
    
    return res;
	
#endif
}

//shl

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	src = src & MASK;
	dest = dest & MASK;
	
	
	uint32_t res = (dest << src) & MASK;
	
	set_PF(res);
	set_SF(res, data_size);
	set_ZF(res, data_size);
	
	
	if(src > data_size)
	{
	    cpu.eflags.CF = 0;
	}
	else
	{
	    cpu.eflags.CF = dest >> (data_size - src) & 1;
	}
	
	return res;
#endif
}

//shr
uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//(stdout);
	//assert(0);
	

    src = src & MASK;	
	dest = dest & MASK;
	uint32_t res = (dest >> src) & MASK;
	
	
	set_PF(res); 
	set_SF(res, data_size);
	set_ZF(res, data_size);
	
	if(src > data_size)
	{
	    cpu.eflags.CF = 0;
	}
	else
	{
	    cpu.eflags.CF = (dest >> (src - 1)) & 1;
	}

	return res;
#endif
}

//sal
uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	src = src & MASK;
	dest = dest & MASK;
	uint32_t res = (dest << src) & MASK;
	
	set_PF(res);
	set_SF(res, data_size);
	set_ZF(res, data_size);
	
	if(src > data_size)
	{
	    cpu.eflags.CF = 0;
	}
	else
	{
	    cpu.eflags.CF = dest >> (data_size - src) & 1;
	}
	
	return res;
#endif
}


//sar
uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	dest = sign_ext(dest & MASK, data_size);
	
	//shift as signed
	int32_t signed_dest = (int32_t) dest; 
    int32_t signed_res = signed_dest >> src;  
    
    uint32_t res = ((uint32_t) signed_res) & MASK; //get unsigned result after shift
    
    cpu.eflags.CF = ((uint32_t)(signed_dest >> (src - 1))) & 0x1;  //set CF
	
	set_PF(res);
	set_SF(res, data_size);
	set_ZF(res, data_size);
	
	return res;
#endif
}


//mod
uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	assert(src != 0);
	uint64_t res = dest % src;
	return res;
	
#endif
}

//imod
int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	assert(src != 0);
	int64_t res = dest % src;
	return res;
#endif
}

//div
// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	assert(src != 0);
	
	uint64_t mask = (uint64_t)0xffffffffffffffff >> (64 - data_size);
	
	src = src & mask;
	dest = dest & mask;
	
	uint32_t res = dest / src;
	return res & MASK;
	
#endif
}


//idiv
// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	assert(src != 0);
	
	int32_t res = dest / src;
	return res;
	
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	
	uint64_t result = 0;
	
	if(data_size == 8)
	{
	    //8bits operation
	    
	    uint16_t al = dest & 0xFF;
	    uint16_t rm8 = src & 0xFF;
	    
	    result = (uint64_t)(al * rm8);
	    cpu.eflags.CF = cpu.eflags.OF = (result >> 8) != 0; 
	}
	else if(data_size == 16)
	{
	    uint32_t ax = dest & 0xFFFF;
	    uint32_t rm16 = src & 0xFFFF;
	    
	    result = (uint64_t)(ax * rm16);
	    cpu.eflags.CF = cpu.eflags.OF = (result >> 16) != 0; 
	    
	}
	else if(data_size == 32)
	{
	    uint64_t eax = (uint64_t)(dest & 0xFFFFFFFF);
	    uint64_t rm32 = (uint64_t)(src & 0xFFFFFFFF);
	    
	    result = eax * rm32;
	    
	    cpu.eflags.CF = cpu.eflags.OF = (result >> 32) != 0; 
	}
	
	return result;
#endif
}


//imul
int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	///printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//fflush(stdout);
	//assert(0);
	int64_t result = 0;

	if(data_size == 8) //8bits
	{
	    int8_t src_8 = (int8_t)src;
        int8_t dest_8 = (int8_t)dest;
        result = (int64_t)(src_8 * dest_8); 
        //cpu.eflags.CF = cpu.eflags.OF = (result >> 8) != 0; 
	}
	else if(data_size == 16)
	{
	    int16_t src_16 = (int16_t)src;
        int16_t dest_16 = (int16_t)dest;
        result = (int64_t)(src_16 * dest_16);
        //cpu.eflags.CF = cpu.eflags.OF = (result >> 16) != 0; 
	}
	else if(data_size == 32)
	{
	    int64_t temp_result = (int64_t)((int64_t)src * (int64_t)dest); 
        int32_t lower = (int32_t)(temp_result & 0xFFFFFFFF);
        int32_t upper = (int32_t)((temp_result >> 32) & 0xFFFFFFFF);
        result = (int64_t)((uint64_t)upper << 32) | (uint32_t)lower;
        //cpu.eflags.CF = cpu.eflags.OF = (result >> 32) != 0; 
	}
	
	return result;
#endif
}
















