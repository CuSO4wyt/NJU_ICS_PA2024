#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	//printf("\nPlease implement page_translate()\n");
	//fflush(stdout);
	//assert(0);

#ifdef IA32_PAGE

//分页模式已经启动
    
    PDE pde;
    pde.val = paddr_read((cpu.cr3.pdb << 12) + (((laddr >> 22) & 0x3ff)<< 2), 4);
    assert(pde.present == 1);
    
    
    PTE pte;
    pte.val = paddr_read((pde.page_frame << 12) + (((laddr >> 12) & 0x3ff) << 2), 4); 
    assert(pte.present == 1);  

    paddr_t paddr = (pte.page_frame << 12) | (laddr & 0xfff);
    return paddr;

#else
    return 0;
    
#endif

#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}