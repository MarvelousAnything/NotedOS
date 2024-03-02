#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <stddef.h>

static inline size_t strlen(const char *s) {
  size_t len = 0;
  while (*s++)
    len++;
  return len;
}

static inline void strcpy(char *dst, const char *src) {
  while(*src)
    *dst++ = *src++;
  *dst = '\0';
} 

static inline void strcat(char *dst, const char *src) {
  while (*dst)
    dst++;
  while(*src)
    *dst++ = *src++;
  *dst = '\0';
}

#endif // !_STRINGS_H_
