#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) // number of page tables to cover the vmem

PDE *get_updir();


PTE VGA_table[NR_PT * NR_PTE] align_to_page;

static void init_pde(PDE *pdir, PTE *ptable)
{
    uint32_t index = 0x0;
    
    pdir[index].val = make_pde(ptable);
}


static void init_pte(PTE *ptable)
{
    uint32_t IndexF = 0x0;
    uint32_t IndexT = 0x0;
    
    while (IndexT < 1024)
    {
        ptable->val = make_pte(IndexF << 12);
        IndexF++;
        ptable++;
        IndexT++;
    }

}



void create_video_mapping()
{
    PDE *P_dir = get_updir();
    PTE *P_table = va_to_pa(VGA_table);
    
    init_pde(P_dir, P_table);
    init_pte(P_table);
}



void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
