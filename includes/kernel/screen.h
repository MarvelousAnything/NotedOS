#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdint.h>


typedef struct screen {
  uint64_t addr;
  uint32_t pitch;
  uint32_t width;
  uint32_t height;
  uint8_t bpp;

} screen_t;

extern screen_t screen;

void init_screen();
void set_pixel(uint32_t x, uint32_t y, uint32_t color);
void clear_screen(uint32_t color);
void draw_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void draw_character(uint32_t x, uint32_t y, char ch, uint32_t color);
void draw_character_scaled(int x, int y, char ch, uint32_t color, int scale);
int char_width(int scale);
int text_width(const char* str, int spacing, int scale);
void draw_string(uint32_t x, uint32_t y, const char *str, uint32_t color, int spacing);
void draw_string_scaled(int x, int y, const char* str, uint32_t color, int spacing, int scale);
void draw_lines(int x, int y, const char *strs[], int lines, uint32_t color, int spacing, int scale);

#endif // !_SCREEN_H_
