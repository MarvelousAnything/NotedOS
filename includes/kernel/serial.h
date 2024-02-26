#ifndef _SERIAL_H_
#define _SERIAL_H_

#define PORT 0x3f8

int init_serial();
unsigned char read_serial();
void write_serial(char a);
void send_string(const char *s);
void send_hex(unsigned int i);
void send_hex_long(unsigned long i);

#endif // !_SERIAL_H_
