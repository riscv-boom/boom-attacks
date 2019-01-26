# BOOM Speculative Attacks

This repository holds all the work-in-progress code used to check if BOOM is susceptible to Spectre attacks.

# Project Members

| Name             | Github Handle |
| ---------------- | ------------- |
| Abraham Gonzalez | abejgonzalez  |
| Ed Younis        | riyt          |
| Ben Korpan       | bkorpan       |
| Jerry Zhao       | jerry123      |

# Further Details

## BOOM Configuration

This is working with the version of BOOM located at https://github.com/riscv-boom/riscv-boom/commit/8bb0e34feedf6d7b2465b10e0e166fec988b0396.

```
Processor Details:

Extra Addition: Default FTQ Size

Fetch Width        : 2
Decode Width       : 2
Issue Width        : 4
ROB Size           : 100

==Dense BTB==
Sets          : 512
Banks         : 2
Ways          : 4
Branch Levels : 2
Tag Size      : 13
Offset Size   : 13

==BIM==
(4 Kbits = 0 kB) Bimodal Table (1024 entries across 2 banks)

==GShare==
(2 kB) GShare Predictor, with 23 bits of history for (2-wide fetch) and 4096 entries.
```

## Implemented Attacks

The following attacks are implemented within the repo.

* Spectre-v1 or Bounds Check Bypass [1]
    * condBranchMispred.c
* Spectre-v2 or Branch Target Injection [1]
    * indirBranchMispred.c

## Not Completed Attacks

The following attacks are in-progress and are not working yet.

* Return Stack Buffer Attack [2]
    * returnStackBuffer.c
    * Main reason why this doesn't work is because the RSB was disconnected in the BPU (commented out). 

# Building the tests

To build you need to run `make`

# Running the Tests

This builds "baremetal" binaries that can directly run on the BOOM configuration that was specified above.

# References

[1] P. Kocher, D. Genkin, D. Gruss, W. Haas, M. Hamburg, M. Lipp, S. Mangard, T. Prescher, M. Schwarz, and Y. Yarom, “Spectre attacks: Exploiting speculative execution,” ArXiv e-prints, Jan. 2018

[2] E. M. Koruyeh, K. N. Khasawneh, C. Song, N. Abu-Ghazaleh, “Spectre Returns! Speculation Attacks using the Return Stack Buffer,” 12th USENIX Workshop on Offensive Technologies, 2018
