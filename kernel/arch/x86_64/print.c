#include "kernel/print.h"
#include "kernel/serial.h"

static void itoa(char *buf, int base, int d) {
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;

  /*  If %d is specified and D is minus, put ‘-’ in the head. */
  if (base == 'd' && d < 0) {
    *p++ = '-';
    buf++;
    ud = -d;
  } else if (base == 'x')
    divisor = 16;

  /*  Divide UD by DIVISOR until UD == 0. */
  do {
    int remainder = ud % divisor;

    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
  } while (ud /= divisor);

  /*  Terminate BUF. */
  *p = 0;

  /*  Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }
}
void printk(const char *format, ...) {
  char **arg = (char **)&format;
  int c;
  char buf[20];

  arg++;

  while ((c = *format++) != 0) {
    if (c != '%') {
      write_serial(c);
    } else {
      char *p, *p2;
      int pad0 = 0, pad = 0;

      c = *format++;
      if (c == '0') {
        pad0 = 1;
        c = *format++;
      }

      if (c >= '0' && c <= '9') {
        pad = c - '0';
        c = *format++;
      }

      switch (c) {
      case 'd':
      case 'u':
      case 'x':
        itoa(buf, c, *((int *)arg++));
        p = buf;
        goto string;
        break;

      case 's':
        p = *arg++;
        if (!p)
          p = "(null)";

      string:
        for (p2 = p; *p2; p2++)
          ;
        for (; p2 < p + pad; p2++) {
          write_serial(pad0 ? '0' : ' ');
        }
        while (*p) {
          write_serial(*p++);
        }
        break;

      default:
        write_serial(*((int *)arg++));
        break;
      }
    }
  }
}
