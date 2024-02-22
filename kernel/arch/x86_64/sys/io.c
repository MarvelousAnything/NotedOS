#include "kernel/sys/io.h"

extern inline unsigned char inb(unsigned short port) {
  unsigned char data = 0;
  __asm__("in %%dx, %%al" : "=a"(data) : "d"(port));
  return data;
}

extern inline void outb(unsigned char value, unsigned short int port) {
  __asm__("out %%al, %%dx" ::"a"(value), "d"(port));
}

extern inline unsigned short inw(unsigned short port) {
  unsigned short data = 0;
  __asm__("in %%dx, %%ax" : "=a"(data) : "d"(port));
  return data;
}

extern inline void outw(unsigned short value, unsigned short port) {
  __asm__("out %%ax, %%dx" : : "a"(value), "d"(port));
}
