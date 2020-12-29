#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)

/* Use the function to get the start address of user page directory. */
// PDE* get_updir();
// PTE vm_pt[NR_PTE] align_to_page;
// void create_video_mapping() {
// 	/* TODO: create an identical mapping from virtual memory area 
// 	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
// 	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
// 	 * some page tables to create this mapping.
// 	 */

// 	PDE *pde = (PDE *)va_to_pa(get_updir());

// 	pde[0].val = make_pde(va_to_pa(vm_pt));
// 	int tot = SCR_SIZE/PAGE_SIZE;
// 	if (SCR_SIZE % PAGE_SIZE != 0) tot++;
// 	int i;
// 	for (i = VMEM_ADDR/PAGE_SIZE;i < VMEM_ADDR/PAGE_SIZE + tot;i ++){
// 		vm_pt[i].val = make_pte(i<<12);
// 	}
// }
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE)
PDE* get_updir();
PTE my_pt[1][NR_PTE] align_to_page;
void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */

	PDE *pdir = get_updir();
	PTE *pt = my_pt[0];
	pdir[0].val = make_pde(va_to_pa(pt));
	int total = SCR_SIZE / PAGE_SIZE + 1;
	uint32_t addr = VMEM_ADDR;
	int i = 0;
	for(; i < total; i++){
		pt[(addr >> 12) & 0x3ff].val = make_pte(addr);
		addr += PAGE_SIZE;
	}

}
void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
		//Log("%d",i);
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

