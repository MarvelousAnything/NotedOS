#include "kernel/serial.h"
#include "kernel/sys/io.h"

int init_serial() {
   outb(0x00, PORT + 1);    // Disable all interrupts
   outb(0x80, PORT + 3);    // Enable DLAB (set baud rate divisor)
   outb(0x03, PORT + 0);    // Set divisor to 3 (lo byte) 38400 baud
   outb(0x00, PORT + 1);    //                  (hi byte)
   outb(0x03, PORT + 3);    // 8 bits, no parity, one stop bit
   outb(0xC7, PORT + 2);    // Enable FIFO, clear them, with 14-byte threshold
   outb(0x0B, PORT + 4);    // IRQs enabled, RTS/DSR set
   outb(0x1E, PORT + 4);    // Set in loopback mode, test the serial chip
   outb(0xAE, PORT + 0);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(PORT + 0) != 0xAE) {
      return 1;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(0x0F, PORT + 4);
   return 0;
}

int serial_received() {
   return inb(PORT + 5) & 1;
}
 
unsigned char read_serial() {
   while (serial_received() == 0);
   return inb(PORT);
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
   outb(a, PORT);
}

void send_string(const char *s) {
  const char *p = s;
  do {
    write_serial(*p);
  } while (*(++p) != 0);
}

void send_hex(unsigned int i) {
  char buf[9];
  buf[8] = 0;
  for (int j = 7; j >= 0; j--) {
    int digit = i & 0xF;
    buf[j] = digit < 10 ? '0' + digit : 'A' + digit - 10;
    i >>= 4;
  }
  send_string(buf);
}

void send_hex_long(unsigned long i) {
  char buf[17];
  buf[16] = 0;
  for (int j = 15; j >= 0; j--) {
    int digit = i & 0xF;
    buf[j] = digit < 10 ? '0' + digit : 'A' + digit - 10;
    i >>= 4;
  }
  send_string(buf);
}
