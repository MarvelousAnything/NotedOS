#include "fmt.h"

void fmt_int(int x, int radix, char *buf, int *n) {
    char buf2[32];
    char *p;
    int neg;
    int offset;
    p = buf2;
    neg = x < 0;

    if (neg)
        x = -x;
    do {
        offset = '0';
        if ((x % radix) > 9)
            offset = 'a' - 10;
        *(p++) = (x % radix) + offset;
    } while (x /= radix);

    if (neg)
        *(p++) = '-';
    do {
        *(buf++) = *(--p);
        (*n)++;
    } while (p != buf2);
}
