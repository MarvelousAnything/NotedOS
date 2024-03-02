#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h>
#include <stdbool.h>

#define IDT_PRESENT         0x80
#define IDT_DPL0            0x00
#define IDT_DPL1            0x20
#define IDT_DPL2            0x40
#define IDT_DPL3            0x60
#define IDT_STORAGE         0x10
#define IDT_INTERRUPT_GATE  0x0E // Use 0x0E for interrupt gates
#define IDT_TRAP_GATE       0x0F // Use 0x0F for trap gates

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtr_t;

typedef struct {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rbp;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	uint64_t vector;
	uint64_t err;

	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__((packed)) int_frame_t;

#define IDT_ENTRIES 256
__attribute__((aligned(0x10)))
static idt_entry_t idt[IDT_ENTRIES];
static idtr_t idtr;

void idt_init(void);
void idt_set_descriptor(uint8_t vector, void* isr, uint16_t sel, uint8_t flags);

#endif // !_IDT_H_
