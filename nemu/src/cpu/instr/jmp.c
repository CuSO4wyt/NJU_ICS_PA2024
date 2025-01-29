#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}


make_instr_func(jmp_short) {
    
    OPERAND rel;
    rel.type = OPR_IMM;        
    rel.sreg = SREG_CS;        
    rel.data_size = 8;         
    rel.addr = eip + 1;        


    operand_read(&rel);

    
    int offset = sign_ext(rel.val, 8);


    print_asm_1("jmp", "b", 1 + 1, &rel);


    cpu.eip += offset;
    
    uint32_t mask = 0x0000FFFF;
    if(data_size == 16)
    {
        cpu.eip = cpu.eip & mask;
    }

    
    return 2;
}


make_instr_func(jmp_near_indirect)
{
    OPERAND rm;
    
    rm.data_size = data_size;
    int len = 1;
    len += modrm_rm(eip + 1, &rm);
    
    operand_read(&rm);
    
    cpu.eip = rm.val & (0xffffffff >> (32 - data_size));


    return 0;
}


make_instr_func(jmp_far_imm)
{

#ifdef IA32_REG

    cpu.cs.val = vaddr_read(cpu.eip + 5, SREG_SS,2);
    load_sreg(1);
    
#endif   

    cpu.eip = vaddr_read(cpu.eip + 1,SREG_SS, 4);
    return 0;


    
}