#include "cpu/intr.h"
#include "cpu/instr.h"

/*
Put the implementations of `int' instructions here.

Special note for `int': please use the instruction name `int_' instead of `int'.
*/

make_instr_func(int_) 
{
   
    
    OPERAND interrupt_num;

    interrupt_num.data_size = 8;      
    interrupt_num.addr = eip + 1;  
    interrupt_num.type = OPR_IMM;      
    interrupt_num.sreg = SREG_CS;     


    operand_read(&interrupt_num);
    raise_sw_intr(interrupt_num.val);


    return 0;
}
