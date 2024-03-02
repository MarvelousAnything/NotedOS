#include "kernel/screen.h"
#include <stdint.h>

extern int font[94][13];

screen_t screen;

void set_pixel(uint32_t x, uint32_t y, uint32_t color) {
  uint32_t *pixel_addr =
      (uint32_t *)(screen.addr + (y * screen.pitch) + (x * screen.bpp / 8));
  *pixel_addr = color;
}

void clear_screen(uint32_t color) {
  for (uint32_t y = 0; y < screen.height; y++) {
    for (uint32_t x = 0; x < screen.width; x++) {
      set_pixel(x, y, color);
    }
  }
}

void draw_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
                    uint32_t color) {
  for (uint32_t dy = 0; dy < height; dy++) {
    for (uint32_t dx = 0; dx < width; dx++) {
      set_pixel(x + dx, y + dy, color);
    }
  }
}

void draw_character(uint32_t x, uint32_t y, char ch, uint32_t color) {
  if (ch < 32 || ch > 125)
    return;
  int index = ch - 32;

  for (int row = 0; row < 13; ++row) {
    int bits = font[index][13 - row - 1];
    for (int col = 0; col < 8; ++col) {
      if (bits & (1 << (7 - col))) {
        set_pixel(x + col, y + row, color);
      }
    }
  }
}

void draw_character_scaled(int x, int y, char ch, uint32_t color, int scale) {
  if (ch < 32 || ch > 125)
    return;            // Check if the character is within the supported range
  int index = ch - 32; // Adjust index to start from 0 for the first printable
                       // character (' ')

  for (int row = 0; row < 13; ++row) {
    for (int row_scale = 0; row_scale < scale; ++row_scale) { // Scale row
      int bits =
          font[index][13 - row - 1]; // Get the row bitmap for the character
      for (int col = 0; col < 8; ++col) {
        if (bits &
            (1 << (7 - col))) { // Check each bit (0-7) within the integer
          for (int col_scale = 0; col_scale < scale;
               ++col_scale) { // Scale column
            set_pixel(x + (col * scale) + col_scale,
                      y + (row * scale) + row_scale,
                      color); // Draw scaled pixel
          }
        }
      }
    }
  }
}

void draw_string(uint32_t x, uint32_t y, const char *str, uint32_t color,
                 int spacing) {
  while (*str) {
    draw_character(x, y, *str, color);
    x += 8 + spacing;
    ++str;
  }
}

void draw_string_scaled(int x, int y, const char *str, uint32_t color,
                        int spacing, int scale) {
  while (*str) {
    draw_character_scaled(x, y, *str, color,
                          scale); // Draw each character in the string scaled
    x += (8 * scale) + spacing;   // Move to the next character position with
                                  // additional spacing and scaling
    ++str;
  }
}

void draw_lines(int x, int y, const char *strs[], int lines, uint32_t color, int spacing, int scale) {
  for (int i = 0; i < lines; i++)
    draw_string_scaled(x, y + (i * (16 + 1)), strs[i], color, spacing, scale);
}

int char_width(int scale) { return 8 * scale; }

int text_width(const char *str, int spacing, int scale) {
  int numChars = 0;
  while (*str) {
    ++numChars;
    ++str;
  }

  if (numChars == 0)
    return 0;
  return (numChars * 8 * scale) + ((numChars - 1) * spacing);
}
