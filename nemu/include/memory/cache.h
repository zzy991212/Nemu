#ifndef __CACHE_H__
#define __CACHE_H__

#include "common.h"

/*
cache block 存储空间的大小为 64B
cache 存储空间的大小为 64KB
8-way set associative
标志位只需要 valid bit 即可
替换算法采用随机方式
write through
not write allocate
*/

#define Cache_L1_Size 64 * 1024
#define Cache_L1_Block_Size 64
#define Cache_L1_Way_Bit 3
#define Cache_Li_Group_Bit 7
#define Cache_L1_Block_Bit 6
#define Cache_L1_Group_Size 1 << Cache_Li_Group_Bit
#define Cache_L1_Way_Size 1 << Cache_L1_Way_Bit


typedef struct{
    uint8_t data[Cache_L1_Block_Size]; // 64B
    uint32_t tag;
    bool valid;
}Cache_L1;

Cache_L1 cache1[Cache_L1_Size/Cache_L1_Block_Size];

void init_cache();

#endif
 
