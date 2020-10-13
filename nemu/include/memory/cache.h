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
#define Cache_L1_Group_Bit 7
#define Cache_L1_Block_Bit 6
#define Cache_L1_Group_Size (1 << Cache_L1_Group_Bit)
#define Cache_L1_Way_Size (1 << Cache_L1_Way_Bit)

#define Test

typedef struct{
    uint8_t data[Cache_L1_Block_Size]; // 64B
    uint32_t tag;
    bool valid;
}Cache_L1;

Cache_L1 cache1[Cache_L1_Size/Cache_L1_Block_Size];

/*
cache block 存储空间的大小为 64B
cache 存储空间的大小为 4MB
16-way set associative
标志位包括 valid bit 和 dirty bit
替换算法采用随机方式
write back
write allocate
*/

#define Cache_L2_Size 4 * 1024 * 1024
#define Cache_L2_Block_Size 64
#define Cache_L2_Way_Bit 4
#define Cache_L2_Group_Bit 12
#define Cache_L2_Block_Bit 6
#define Cache_L2_Group_Size (1 << Cache_L2_Group_Bit)
#define Cache_L2_Way_Size (1 << Cache_L2_Way_Bit)

typedef struct{
    uint8_t data[Cache_L2_Block_Size]; // 64B
    uint32_t tag;
    bool valid;
    bool dirty;
}Cache_L2;

Cache_L2 cache2[Cache_L2_Size/Cache_L2_Block_Size];

#ifdef Test
int test_time;
#endif

void init_cache();
int read_cache1(hwaddr_t);
void write_cache1(hwaddr_t, size_t, uint32_t);
void write_cache2(hwaddr_t, size_t, uint32_t);
int read_cache2(hwaddr_t);
#endif
 
