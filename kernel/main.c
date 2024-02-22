#include "kernel/serial.h"
#include "kernel/vga.h"

_Noreturn void kernel_main(void) {
  if (init_serial()) {
  }
  send_string("Hello World!\n");
  send_string("Hello World Again!\n");
  init_vga();
  vga_puts("hello", 5);
  while (1)
    asm("hlt");
}
