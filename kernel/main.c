#include "kernel/serial.h"

void kernel_main() {
  init_serial();
  send_string("Hello World\n");
  send_string("Hello World\n");
  send_string("Hello World\n");
  send_string("Hello World\n");
  // write_serial('a');
  while (1)
    asm("hlt");
}
