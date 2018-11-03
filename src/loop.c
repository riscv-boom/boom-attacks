#include <stdio.h>
#include "encoding.h"

int dummy = 0;
int main (void){
    // Enable user/supervisor use of perf counters             
    write_csr(mcounteren, -1);                                
    write_csr(scounteren, -1);   

    // Set the HPM event selectors as desired for your specific micro-architecture
    write_csr(mhpmevent3, 0x0401); // read in the br misspred
    write_csr(mhpmevent4, 0x0801); // read in the br jalr mispred
    write_csr(mhpmevent5, 0x2001); // read in the br resolve
    write_csr(mhpmevent6, 0x0200); // read in nop
    write_csr(mhpmevent7, 0x0400); // read in nop
    write_csr(mhpmevent8, 0x0800); // read in nop

    //write_csr(mhpmevent3,  0x0001); // read in the br misspred
    //write_csr(mhpmevent4,  0x0002); // read in the br jalr mispred
    //write_csr(mhpmevent5,  0x0004); // read in the br resolve
    //write_csr(mhpmevent6,  0x0008);
    //write_csr(mhpmevent7,  0x0010);
    //write_csr(mhpmevent8,  0x0020);
    //write_csr(mhpmevent9,  0x0040);
    //write_csr(mhpmevent10, 0x0080);
    //write_csr(mhpmevent12, 0x0100);
    //write_csr(mhpmevent13, 0x0200);
    //write_csr(mhpmevent14, 0x0400);
    //write_csr(mhpmevent15, 0x0800);
    //write_csr(mhpmevent16, 0x1000);
    //write_csr(mhpmevent17, 0x2000);
    //write_csr(mhpmevent18, 0x4000);
    //write_csr(mhpmevent19, 0x8000);
    
    //write_csr(mhpmevent3,  0x2002); // read in the br misspred
    //write_csr(mhpmevent4,  0x4002); // read in the br jalr mispred
    //write_csr(mhpmevent5,  0x8002); // read in the br resolve
    //write_csr(mhpmevent6,  0x0103);
    //write_csr(mhpmevent7,  0x0203);
    //write_csr(mhpmevent8,  0x0403);
    //write_csr(mhpmevent9,  0x0803);
    //write_csr(mhpmevent10, 0x1003);
    //write_csr(mhpmevent12, 0x2003);
    //write_csr(mhpmevent13, 0x4003);
    //write_csr(mhpmevent14, 0x8003);
    //write_csr(mhpmevent15, 0x0104);
    //write_csr(mhpmevent16, 0x0204);
    //write_csr(mhpmevent17, 0x0404);
    //write_csr(mhpmevent18, 0x0804);
    //write_csr(mhpmevent19, 0x1004);

    for(long i = 0; i < 10000; ++i){
//        if (i < 1000000){
            asm("");
//            dummy |= dummy + i;
//        }
    }

    printf("(BR Mispred, BR JALR Mispred, BR Resolve): (%lu, %lu, %lu)\n", read_csr(hpmcounter3), read_csr(hpmcounter4), read_csr(hpmcounter5));
    printf("(0,0,0): (%lu, %lu, %lu)\n", read_csr(hpmcounter6), read_csr(hpmcounter7), read_csr(hpmcounter8));
    printf("(cycle): (%lu)\n", read_csr(cycle));
    printf("(instret): (%lu)\n", read_csr(instret));
    //printf("(cntr3): (%u)\n", read_csr(mhpmcounter3));
    //printf("(cntr4): (%u)\n", read_csr(mhpmcounter4));
    //printf("(cntr5): (%u)\n", read_csr(mhpmcounter5));
    //printf("(cntr6): (%u)\n", read_csr(mhpmcounter6));
    //printf("(cntr7): (%u)\n", read_csr(mhpmcounter7));
    //printf("(cntr8): (%u)\n", read_csr(mhpmcounter8));
    //printf("(cntr9): (%u)\n", read_csr(mhpmcounter9));
    //printf("(cntr10): (%u)\n", read_csr(mhpmcounter10));
    //printf("(cntr12): (%u)\n", read_csr(mhpmcounter12));
    //printf("(cntr13): (%u)\n", read_csr(mhpmcounter13));
    //printf("(cntr14): (%u)\n", read_csr(mhpmcounter14));
    //printf("(cntr15): (%u)\n", read_csr(mhpmcounter15));
    //printf("(cntr16): (%u)\n", read_csr(mhpmcounter16));
    //printf("(cntr17): (%u)\n", read_csr(mhpmcounter17));
    //printf("(cntr18): (%u)\n", read_csr(mhpmcounter18));
    //printf("(cntr19): (%u)\n", read_csr(mhpmcounter19));
}
