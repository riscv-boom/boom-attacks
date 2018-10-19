GCC=riscv64-unknown-elf-gcc
OBJDUMP=riscv64-unknown-elf-objdump
CFLAGS=-mcmodel=medany -std=gnu99 -O2 -fno-common -fno-builtin-printf -Wall
LDFLAGS=-static -nostdlib -nostartfiles -lgcc

SRC=src
INC=inc
LNK=link
OBJ=obj
BIN=bin
DMP=dump

PROGRAMS = condBranchMispred returnStackBuffer

default: $(addprefix $(BIN)/,$(addsuffix .riscv,$(PROGRAMS)))

dumps: $(addprefix $(DMP)/,$(addsuffix .dump,$(PROGRAMS)))

$(OBJ)/%.o: $(SRC)/%.c
	mkdir -p $(OBJ)
	$(GCC) $(CFLAGS) -I$(INC) -c $< -o $@

$(BIN)/%.riscv: $(OBJ)/%.o $(OBJ)/crt.o $(OBJ)/syscalls.o $(LNK)/link.ld
	mkdir -p $(BIN)
	$(GCC) -T $(LNK)/link.ld $(LDFLAGS) $< $(OBJ)/crt.o $(OBJ)/syscalls.o -o $@

$(DMP)/%.dump: $(BIN)/%.riscv
	mkdir -p $(DMP)
	$(OBJDUMP) -D $< > $@

clean:
	rm -rf $(BIN) $(OBJ) $(DMP)
