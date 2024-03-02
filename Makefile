CPU_ARCH = x86_64
CC = $(CPU_ARCH)-elf-gcc
CXX = $(CPU_ARCH)-elf-g++
LD = $(CPU_ARCH)-elf-ld
ASM = nasm

CC_FLAGS = -c \
					 -mno-red-zone \
					 -mno-mmx \
					 -mno-sse \
					 -mno-sse2 \
					 -ffreestanding \
					 -g \
					 -Wall \
					 -Wextra \
					 -std=gnu11 \
					  -fno-stack-protector \
					  -fno-stack-check \
					  -fno-lto \
					  -fno-PIE \
					  -fno-PIC \
					  -m64 \
					  -march=x86-64 \
					  -mabi=sysv \
					  -mcmodel=kernel \
					  -mno-80387 \
					  -mno-red-zone \
					  -DPRINTF_DISABLE_SUPPORT_FLOAT \
					  -DHEAP_ACCESSABLE \
					  -msse \
					  -mgeneral-regs-only \
					  -DSUPPORT_FLOAT \
					  -Wimplicit-function-declaration \
					  -Wdiv-by-zero \
					  -Wunused-variable \
					 -I includes \
					 -I build/limine
ASM_FLAGS = -f elf64 -F dwarf -g -Wall
LD_FLAGS = -T linker.ld -m elf_x86_64 -nostdlib -static -z max-page-size=0x1000 -Map=$(OUT_DIR)/kernel.map
ASM_SRCS =
C_SRCS = 

OUT_DIR = build
TARGET = $(OUT_DIR)/kernel.elf
DISK_DIR = $(OUT_DIR)/disk

OBJS = $(patsubst %.asm, $(OUT_DIR)/%.asm.o, $(ASM_SRCS))
OBJS += $(patsubst %.c, $(OUT_DIR)/%.c.o, $(C_SRCS))

.PHONY: all kernel clean

all: limine kernel disk

include limine.mk
include disk.mk
include std/std.mk
include kernel/kernel.mk

kernel: limine $(OBJS)
	@echo "Linking program ..."
	$(LD) $(LD_FLAGS) $(OBJS) -o $(TARGET)
	@echo "Program linked, placed @ $(TARGET)"

clean:
	@echo "Cleaning build files ..."
	-rm -rf $(OUT_DIR)
	@echo "Cleaning done!"

$(OUT_DIR)/%.c.o: %.c
	@echo "Compiling $< into $@"
	@mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -o $@ $^

$(OUT_DIR)/%.asm.o: %.asm
	@echo "Assembling files"
	@mkdir -p $(@D)
	$(ASM) $(ASM_FLAGS) -o $@ $^
