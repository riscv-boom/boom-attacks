#include <stdio.h>
#include <stdint.h> 

#define L1_SETS 64
#define L1_SET_BITS 6 // note: this is log2Ceil(L1_SETS)
#define L1_WAYS 8 //note: this looks like there are 8 ways
#define L1_BLOCK_SZ_BYTES 64
#define L1_BLOCK_BITS 6 // note: this is log2Ceil(L1_BLOCK_SZ_BYTES)
#define L1_SZ_BYTES (L1_SETS*L1_WAYS*L1_BLOCK_SZ_BYTES)
#define FULL_MASK 0xFFFFFFFFFFFFFFFF
#define OFF_MASK (~(FULL_MASK << L1_BLOCK_BITS))
#define TAG_MASK (FULL_MASK << (L1_SET_BITS + L1_BLOCK_BITS))
#define SET_MASK (~(TAG_MASK | OFF_MASK))

uint8_t array[0xF000 + L1_SZ_BYTES];

// ubench: test if there is multiple misses to the same idx
// tests for if the specbuffer handles the same idx nicely
int main(void){
    uint64_t start, diff;
    uint8_t dummy;

    uint64_t alignedMem = (((uint64_t)&array) + L1_SZ_BYTES) & TAG_MASK;

    dummy = *((uint8_t*) alignedMem + 0x0000);
    dummy = *((uint8_t*) alignedMem + 0x1000);
    dummy = *((uint8_t*) alignedMem + 0x2000);
    dummy = *((uint8_t*) alignedMem + 0x3000);
    dummy = *((uint8_t*) alignedMem + 0x4000);
    dummy = *((uint8_t*) alignedMem + 0x5000);
    dummy = *((uint8_t*) alignedMem + 0x6000);
    dummy = *((uint8_t*) alignedMem + 0x7000);
    dummy = *((uint8_t*) alignedMem + 0x8000);
    dummy = *((uint8_t*) alignedMem + 0x9000);
    dummy = *((uint8_t*) alignedMem + 0xa000);
    dummy = *((uint8_t*) alignedMem + 0xb000);
    dummy = *((uint8_t*) alignedMem + 0xc000);
    dummy = *((uint8_t*) alignedMem + 0xd000);
    dummy = *((uint8_t*) alignedMem + 0xe000);
    dummy = *((uint8_t*) alignedMem + 0xf000);

    printf( "Results: Time to run: (%lu cycles)\n", diff);
}
