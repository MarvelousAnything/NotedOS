#ifndef LOG_H
#define LOG_H

#include <kernel/serial.h>
#include <kernel/vga.h>

#define LOG_BUFFER_SIZE 1024

void kernel_log(const char *s);

#endif // !LOG_H