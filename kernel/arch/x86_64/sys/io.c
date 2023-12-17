#include "kernel/sys/io.h"

extern inline unsigned char inb(unsigned short int port) {
  unsigned char data = 0;
  __asm__ __volatile__ ("inb %%dx, %%al" : "=a" (data) : "d" (port));
  return data;
}

extern inline void outb(unsigned char value, unsigned short int port) {
  __asm__ __volatile__ ("outb %%al, %%dx" :: "a" (value), "d" (port));
}
