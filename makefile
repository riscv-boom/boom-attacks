###############################################################################
# Makefile for boom-attacks
###########################
# Makes baremetal executables to run on BOOM
###############################################################################
# Folders
SRC=src
INC=inc
LNK=link
OBJ=obj
BIN=bin
DMP=dump
DEP=dep

# Commands and flags
GCC=riscv64-unknown-elf-gcc
OBJDUMP=riscv64-unknown-elf-objdump
CFLAGS=-mcmodel=medany -std=gnu99 -O2 -fno-common -fno-builtin-printf -Wall -I$(INC)
LDFLAGS=-static -nostdlib -nostartfiles -lgcc
DEPFLAGS=-MT $@ -MMD -MP -MF $(DEP)/$*.d

# Programs to compile
PROGRAMS=condBranchMispred returnStackBuffer

# Include dependencies
-include $(addprefix $(DEP)/,$(addsuffix .d,$(PROGRAMS)))

default: $(addprefix $(BIN)/,$(addsuffix .riscv,$(PROGRAMS)))

dumps: $(addprefix $(DMP)/,$(addsuffix .dump,$(PROGRAMS)))

# Build object files
$(OBJ)/%.o: $(SRC)/%.S
	mkdir -p $(OBJ)
	$(GCC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.c
	mkdir -p $(OBJ)
	mkdir -p $(DEP)
	$(GCC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Build executable
$(BIN)/%.riscv: $(OBJ)/%.o $(OBJ)/crt.o $(OBJ)/syscalls.o $(LNK)/link.ld
	mkdir -p $(BIN)
	$(GCC) -T $(LNK)/link.ld $(LDFLAGS) $< $(OBJ)/crt.o $(OBJ)/syscalls.o -o $@

# Build dump
$(DMP)/%.dump: $(BIN)/%.riscv
	mkdir -p $(DMP)
	$(OBJDUMP) -D $< > $@

# Remove all generated files
clean:
	rm -rf $(BIN) $(OBJ) $(DMP) $(DEP)
