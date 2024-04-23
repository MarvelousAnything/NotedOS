#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <sys/types.h>
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern volatile unsigned int row; 
extern volatile unsigned int col; 
extern volatile uint8_t color;
extern volatile uint16_t *vga_buffer;

enum vga_color {
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3,
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHT_GREY = 7,
    VGA_DARK_GREY = 8,
    VGA_LIGHT_BLUE = 9,
    VGA_LIGHT_GREEN = 10,
    VGA_LIGHT_CYAN = 11,
    VGA_LIGHT_RED = 12,
    VGA_LIGHT_MAGENTA = 13,
    VGA_LIGHT_BROWN = 14,
    VGA_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t) uc | (uint16_t) color << 8;
}

void init_vga();

void vga_setcolor(uint8_t c);
void vga_putentryat(char c, uint8_t color, size_t x, size_t y);

void vga_putc(char c);
void vga_puts(const char *str);
void vga_puti(int i);
void vga_putx(int i);

#endif // !VGA_H
