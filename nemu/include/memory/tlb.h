#ifndef __TLB_H__
#define __TLB_H__

#include "common.h"

#define TLB_SIZE 64
typedef struct{
    bool valid;
    uint32_t tag;
    uint32_t page_num;
}TLB;

TLB tlb[TLB_SIZE];

void init_tlb();
int read_tlb(uint32_t addr);
void write_tlb(uint32_t lnaddr,uint32_t hwaddr);

#endif
 
