#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
#ifdef IA32_SEG
	return offset + cpu.segReg[sreg].base;
#else
	return 0;
#endif

}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
#ifdef IA32_SEG
    
    assert(sreg >= 0);
    assert(sreg < 6);
    
    SegDesc seg;
    uint32_t index = cpu.segReg[sreg].index;
    
    seg.val[0] = laddr_read(cpu.gdtr.base + index * 8, 4);
    seg.val[1] = laddr_read(cpu.gdtr.base + index * 8 + 4, 4);
    
    uint32_t base = (seg.base_31_24 << 24) | (seg.base_23_16 << 16) | (seg.base_15_0);
   
    assert(base == 0);
    
    cpu.segReg[sreg].base = base;
    
    uint32_t limit = seg.limit_15_0 | (seg.limit_19_16 << 16);
    
    assert(limit == 0xFFFFF);
    
    cpu.segReg[sreg].limit = limit;
    
    
    cpu.segReg[sreg].soft_use = seg.soft_use;
    cpu.segReg[sreg].type = seg.type;
    cpu.segReg[sreg].privilege_level = seg.privilege_level;
    
    assert(seg.granularity == 1);
    
#endif
}
