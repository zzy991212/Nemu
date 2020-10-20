#include "common.h"
#include "memory/tlb.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "burst.h"

void init_tlb(){
    int i;
    for (i = 0;i < TLB_SIZE;i++){
        tlb[i].valid = 0;
    }
}

int read_tlb(uint32_t addr){
    int dir = addr >> 12;
    int i;
    for (i = 0; i < TLB_SIZE;i++){
        if (tlb[i].tag == dir && tlb[i].valid == 1){
            return i;
        }
    }
    return -1;
}

void write_tlb(uint32_t lnaddr,uint32_t hwaddr){
    int dir = lnaddr >> 12;
    int page = hwaddr >> 12;
    int i;
    for (i = 0; i < TLB_SIZE;i++){
        if (!tlb[i].valid){
            tlb[i].valid = 1;
            tlb[i].tag = dir;
            tlb[i].page_num = page;
            return;
        }
    }
    srand(time(0));
    i = rand()%TLB_SIZE;
    tlb[i].valid = 1;
    tlb[i].tag = dir;
    tlb[i].page_num = page;
}