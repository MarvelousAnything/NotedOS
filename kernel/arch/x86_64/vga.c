#include "kernel/vga.h"
#include <stdint.h>
#include <sys/_types/_size_t.h>

volatile unsigned int row; 
volatile unsigned int col; 
volatile uint8_t color;
volatile uint16_t *vga_buffer;

void init_vga() {
  row = 0;
  col = 0;
  color = vga_entry_color(VGA_LIGHT_GREY, VGA_BLACK);
  vga_buffer = (volatile uint16_t *)VGA_ADDRESS;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      vga_buffer[index] = vga_entry(' ', color);
    }
  }
}

void vga_setcolor(uint8_t c) {
  color = c;
}

void vga_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  vga_buffer[index] = vga_entry(c, color);
}

void vga_putc(char c) {
  vga_putentryat(c, color, col, row);
  if (++col == VGA_WIDTH) {
    col = 0;
    if (++row == VGA_HEIGHT)
      row = 0;
  }
}

void vga_puts(const char *str, size_t size) {
  for (size_t i = 0; i < size; i++)
    vga_putc(str[i]);
}
