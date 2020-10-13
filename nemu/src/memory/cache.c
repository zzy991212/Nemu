#include "common.h"
#include "memory/cache.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "burst.h"

// Cache_L1
// #define Cache_L1_Size 64 * 1024
// #define Cache_L1_Block_Size 64
// #define Cache_L1_Way_Bit 3
// #define Cache_L1_Group_Bit 7
// #define Cache_L1_Block_Bit 6
// #define Cache_L1_Group_Size 1 << Cache_Li_Group_Bit
// #define Cache_L1_Way_Size 1 << Cache_L1_Way_Bit

void ddr3_read_replace(hwaddr_t addr, void *data);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);


void init_cache(){
    int i;
    for (i = 0;i < Cache_L1_Size / Cache_L1_Block_Size; i ++) {
        cache1[i].valid = 0;
    }
}

int read_cache1(hwaddr_t addr){
    uint32_t group_idx = (addr >> Cache_L1_Block_Bit) & (Cache_L1_Group_Size - 1);
    uint32_t tag = (addr >> (Cache_L1_Group_Bit + Cache_L1_Block_Bit));
    uint32_t block_start = (addr >> Cache_L1_Block_Bit) << Cache_L1_Block_Bit;

    int i,group = group_idx * Cache_L1_Way_Size;
    for (i = group + 0;i < group + Cache_L1_Way_Size;i ++){
        if (cache1[i].valid == 1 && cache1[i].tag == tag){// READ HIT
            return i;
        }
    }
    // Random
    srand(time(0));
    i = group + rand() % Cache_L1_Way_Size;
    int j;
    for (j = 0;j < Cache_L1_Block_Size / BURST_LEN;j ++){
        ddr3_read_replace(block_start + BURST_LEN * j, cache1[i].data + BURST_LEN * j);
    }
    cache1[i].valid = 1;
    cache1[i].tag = tag;
    return i;
}

void write_cache1(hwaddr_t addr, size_t len, uint32_t data){
    uint32_t group_idx = (addr >> Cache_L1_Block_Bit) & (Cache_L1_Group_Size - 1);
    uint32_t tag = (addr >> (Cache_L1_Group_Bit + Cache_L1_Block_Bit));
    uint32_t offset = addr & (Cache_L1_Block_Size - 1);
    printf("111\n");
    int i,group = group_idx * Cache_L1_Group_Size;
    for (i = group + 0;i < group + Cache_L1_Way_Size;i ++){
        if (cache1[i].valid == 1 && cache1[i].tag == tag){// WRITE HIT
            /*write through*/
            if (offset + len > Cache_L1_Block_Size){
                dram_write(addr,Cache_L1_Block_Size - offset,data);
                memcpy(cache1[i].data + offset, &data, Cache_L1_Block_Size - offset);
                write_cache1(addr + Cache_L1_Block_Size - offset,len - (Cache_L1_Block_Size - offset),data >> (Cache_L1_Block_Size - offset));
            }else {
                dram_write(addr,len,data);
                memcpy(cache1[i].data + offset, &data, len);
            }
            return;
        }
    }
    /*not write allocate*/
    dram_write(addr,len,data);
}