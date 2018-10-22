#ifndef RVUTIL_H
#define RVUTIL_H

#define RDCYCLE ({                            \
    int out;                                  \
    asm volatile ("rdcycle %0" : "=r" (out)); \
    out;                                      \
})

#endif
