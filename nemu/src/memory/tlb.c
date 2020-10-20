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