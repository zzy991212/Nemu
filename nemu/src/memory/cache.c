#include "common.h"
#include "memory/cache.h"

void init_cache(){
    int i;
    for (i = 0;i < Cache_L1_Size / Cache_L1_Block_Size; i ++) {
        cache1[i].valid = 0;
        cache1[i].tag = 0;
        memset(cache1[i].data,0,Cache_L1_Block_Size);
    }
}
