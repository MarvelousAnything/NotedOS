#include <kernel/printf.h>
#include <kernel/sys/idt.h>
#include <stdint.h>

extern void* isr_stub_table[];


static const char* exception_messages[] = {
  "Division By Zero Exception",       // Vector 0
  "Debug Exception",                  // Vector 1
  "Non Maskable Interrupt",           // Vector 2
  "Breakpoint Exception",             // Vector 3
  "Overflow Exception",               // Vector 4
  "Bound Range Exceeded Exception",   // Vector 5
  "Invalid Opcode Exception",         // Vector 6
  "Device Not Available Exception",   // Vector 7
  "Double Fault",                     // Vector 8
  "Coprocessor Segment Overrun",      // Vector 9 (obsolete in modern processors)
  "Invalid TSS Exception",            // Vector 10
  "Segment Not Present",              // Vector 11
  "Stack-Segment Fault",              // Vector 12
  "General Protection Fault",         // Vector 13
  "Page Fault",                       // Vector 14
  "Reserved",                         // Vector 15
  "x87 Floating-Point Exception",     // Vector 16
  "Alignment Check Exception",        // Vector 17
  "Machine Check Exception",          // Vector 18
  "SIMD Floating-Point Exception"     // Vector 19
};

#define EXCEPTION_COUNT (sizeof(exception_messages) / sizeof(exception_messages[0]))

void idt_init() {
  idtr.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
  idtr.base = (uint64_t) &idt;

  for (uint8_t vector = 0; vector < 32; vector++) {
    idt_set_descriptor(vector, isr_stub_table[vector], 0x28, 0x8E);
  }

  __asm__ volatile ("lidt %0" : : "m"(idtr));
  __asm__ volatile ("sti");
}

void idt_set_descriptor(uint8_t vector, void* isr, uint16_t sel, uint8_t flags) {
  idt_entry_t* descriptor = &idt[vector];

  descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
  descriptor->kernel_cs      = sel;
  descriptor->ist            = 0;
  descriptor->attributes     = flags;
  descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
  descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
  descriptor->reserved       = 0;
}

static bool test = true;
static int a;

void exception_handler(int_frame_t frame) {
  if (frame.vector < EXCEPTION_COUNT) {
    printf("Exception occurred: %s\n", exception_messages[frame.vector]);
  } else {
    printf("Unknown exception vector: %llu\n", frame.vector);
  }
  __asm__ volatile ("cli; hlt");
}
