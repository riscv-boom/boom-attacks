#ifndef CACHE_H
#define CACHE_H

// cache values
// TODO: check that these parameters are right
#define L1_SETS 64
#define L1_SET_BITS 6 // note: this is log2Ceil(L1_SETS)
#define L1_WAYS 4
#define L1_BLOCK_SZ_BYTES 64
#define L1_BLOCK_BITS 6 // note: this is log2Ceil(L1_BLOCK_SZ_BYTES)
#define L1_SZ_BYTES (L1_SETS*L1_WAYS*L1_BLOCK_SZ_BYTES)
#define FULL_MASK 0xFFFFFFFFFFFFFFFF
#define OFF_MASK (FULL_MASK << L1_BLOCK_BITS)
#define TAG_MASK (FULL_MASK << (L1_SET_BITS + L1_BLOCK_BITS))
#define IDX_MASK (~(TAG_MASK | OFF_MASK))

/* ----------------------------------
 * |                  Cache address |
 * ----------------------------------
 * |       tag |      idx |  offset |
 * ----------------------------------
 * | 63 <-> 12 | 11 <-> 6 | 5 <-> 0 |
 * ----------------------------------
 */

// setup array size of cache to "put" in the cache on $ flush
// gaurantees contiguous set of addrs that is sz of cache
uint8_t badMem[2*L1_SZ_BYTES]

/**
 * Flush the cache of the address given since RV64 does not have a
 * clflush type of instruction. Clears any set that has the same idx bits
 * as the address input range
 *
 * @param addr starting address to clear the cache
 * @param sz size of the data to remove in bytes
 */
void flushCache(uint64_t addr, uint64_t sz){
    // get the starting address that you want to clear from
    uint64_t baseAddr = addr & TAG_MASK; 

    // find out the amount of blocks you want to clear
    uint64_t numBlocksClear = sz >> L1_BLOCK_SZ_BYTES;
    if (sz & IDX_MASK != 0){
        numBlocksClear += 1;
    }

    uint8_t dummy = 0;
    for (uint64_t i = 0; i < numBlocksClear; ++i){
        for(uint64_t j = 0; j < L1_WAYS; ++j){
            // this is aligned to the cache, now dump the sets
            uint8_t* newMem = ((&badMem + L1_SZ_BYTES) & (TAG_MASK));
            dummy &= *( newMem + ((addr & IDX_MASK) >> L1_SET_BITS)*BLOCK_SZ + (j*L1_BLOCK_SZ_BYTES*NUM_SETS) )
        }
    }
}

#endif
