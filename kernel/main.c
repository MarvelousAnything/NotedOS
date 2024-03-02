#include "kernel/print.h"
#include "kernel/screen.h"
#include "kernel/serial.h"
#include "kernel/sys/idt.h"
#include <limine.h>
#include <std/strings.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

LIMINE_BASE_REVISION(1)

struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 0
};

static void hcf(void) {
  asm("cli");
  for (;;) {
    asm("hlt");
  }
}

_Noreturn void _start(void) {
  init_serial();
  idt_init();

  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    printk("Limine base revision not supported!\n");
    hcf();
  }

  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    printk("Framebuffer request could not get a good response!\n");
    hcf();
  }

  struct limine_framebuffer *framebuffer =
      framebuffer_request.response->framebuffers[0];

  screen.addr = (uint64_t)framebuffer->address;
  screen.width = framebuffer->width;
  screen.height = framebuffer->height;
  screen.pitch = framebuffer->pitch;
  screen.bpp = framebuffer->bpp;

  clear_screen(0x0000ff);
  draw_lines(10, 10,
             (const char *[]){"reverse' :: [a] -> [a] -> [a]",
                              "reverse' [] ys = ys",
                              "reverse' (x:xs) ys = reverse' xs (x : ys)",
                              "Hi GWYNNIE"
                             },
             4, 0x0, 1, 1);


  asm("cli");
  for (;;)
    asm("hlt");
}
