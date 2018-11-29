#include <stdio.h>
#include <stdint.h> 
#include "encoding.h"
#include "cache.h"

#define TRAIN_TIMES 6 // assumption is that you have a 2 bit counter in the predictor
#define ROUNDS 1 // run the train + attack sequence X amount of times (for redundancy)

uint8_t array1_sz = 1;
uint8_t array[L1_SZ_BYTES + 0xa000];

void victimFunc(uint8_t idx){
    uint8_t dummy = 2;

    uint64_t alignedMem = (((uint64_t)&array) + L1_SZ_BYTES) & TAG_MASK;

    // stall array1_sz by doing div operations (operation is (array1_sz << 4) / (2*4))
    array1_sz =  array1_sz << 4;
    asm("fcvt.s.lu	fa4, %[in]\n"
        "fcvt.s.lu	fa5, %[inout]\n"
        "fdiv.s	fa5, fa5, fa4\n"
        "fdiv.s	fa5, fa5, fa4\n"
        "fdiv.s	fa5, fa5, fa4\n"
        "fdiv.s	fa5, fa5, fa4\n"
        "fcvt.lu.s	%[out], fa5, rtz\n"
        : [out] "=r" (array1_sz)
        : [inout] "r" (array1_sz), [in] "r" (dummy)
        : "fa4", "fa5");

    if (idx == array1_sz){
        dummy = *((uint8_t*) alignedMem + 0x8000);
    }
    else {
        dummy = *((uint8_t*) alignedMem + 0x0000);
        dummy = *((uint8_t*) alignedMem + 0x1100);
        dummy = *((uint8_t*) alignedMem + 0x2200);
        dummy = *((uint8_t*) alignedMem + 0x3300);
        dummy = *((uint8_t*) alignedMem + 0x4400);
        dummy = *((uint8_t*) alignedMem + 0x5500);
        dummy = *((uint8_t*) alignedMem + 0x6600);
        dummy = *((uint8_t*) alignedMem + 0x7700);
    }
}

// ubench: have multiple spec loads hit in different idx's
// this should fill up the mshrs
// these spec loads should resolve
int main(void){
    uint64_t start, diff;
    uint8_t dummy;
    uint8_t randIdx, attackIdx, passInIdx;
    
    attackIdx = 0;

    uint64_t alignedMem = (((uint64_t)&array) + L1_SZ_BYTES) & TAG_MASK;

    for(int64_t j = ((TRAIN_TIMES+1)*ROUNDS)-1; j >= 0; --j){
        // bit twiddling to set passInIdx=randIdx or to attackIdx after TRAIN_TIMES iterations
        // avoid jumps in case those tip off the branch predictor
        // note: randIdx changes everytime the atkRound changes so that the tally does not get affected
        //       training creates a false hit in array2 for that array1 value (you want this to be ignored by having it changed)
        randIdx = 1;
        passInIdx = ((j % (TRAIN_TIMES+1)) - 1) & ~0xFFFF; // after every TRAIN_TIMES set passInIdx=...FFFF0000 else 0
        passInIdx = (passInIdx | (passInIdx >> 16)); // set the passInIdx=-1 or 0
        passInIdx = randIdx ^ (passInIdx & (attackIdx ^ randIdx)); // select randIdx or attackIdx 

        // set of constant takens to make the BHR be in a all taken state
        for(uint64_t k = 0; k < 30; ++k){
            asm("");
        }

        // call function to train or attack
        start = rdcycle();
        victimFunc(passInIdx);
    }

    dummy = *((uint8_t*) alignedMem + 0x3000);

    diff = rdcycle() - start;
    printf( "Results: Time to run: (%lu cycles)\n", diff);
}
