CPU_ARCH = x86_64
CC = $(CPU_ARCH)-elf-gcc
CXX = $(CPU_ARCH)-elf-g++
LD = $(CPU_ARCH)-elf-ld
ASM = nasm

CC_FLAGS = -c -fno-pie -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -ffreestanding -g -m32 -I includes
ASM_FLAGS = -f elf32 -F dwarf -g -Wall
LD_FLAGS = -T linker.ld -m elf_i386 -nostdlib -static --no-dynamic-linker -Map=$(OUT_DIR)/kernel.map
ASM_SRCS =
C_SRCS = 

OUT_DIR = build
TARGET = $(OUT_DIR)/kernel.elf
DISK_DIR = $(OUT_DIR)/disk

OBJS = $(patsubst %.asm, $(OUT_DIR)/%.asm.o, $(ASM_SRCS))
OBJS += $(patsubst %.c, $(OUT_DIR)/%.c.o, $(C_SRCS))

.PHONY: all kernel clean

all: kernel limine disk

include limine.mk
include disk.mk
include kernel/kernel.mk

kernel: $(OBJS)
	@echo "Linking program ..."
	$(LD) $(LD_FLAGS) $(OBJS) -o $(TARGET)
	@echo "Program linked, placed @ $(TARGET)"

clean:
	@echo "Cleaning build files ..."
	-rm -rf $(OUT_DIR)
	@echo "Cleaning done!"

$(OUT_DIR)/%.c.o: %.c
	@echo "Compiling C sources"
	@mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -o $@ $^

$(OUT_DIR)/%.asm.o: %.asm
	@echo "Assembling files"
	@mkdir -p $(@D)
	$(ASM) $(ASM_FLAGS) -o $@ $^
