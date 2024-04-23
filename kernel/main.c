#include "kernel/screen.h"
#include "kernel/serial.h"
#include "kernel/sys/idt.h"
#include <limine.h>
#include <std/strings.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/printf.h>

LIMINE_BASE_REVISION(1)

struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 1
};

struct limine_memmap_request memmap_request = {
  .id = LIMINE_MEMMAP_REQUEST,
  .revision = 1
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
    printf("Limine base revision not supported!\n");
    hcf();
  }

  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    printf("Framebuffer request could not get a good response!\n");
    hcf();
  }

  if (memmap_request.response == NULL) {
    printf("memmap response is null!\n");
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
                              "HI JAKE!!!!"
                             },
             4, 0x0, 1, 1);

  char *types[] = {"usable", "reserved", "acpi reclaimable", "acpi nvs", "bad memory", "bootloader reclaimable", "kernel and modules", "framebuffer"};
  printf("memmap:\n");
  for (uint64_t i = 0; i < memmap_request.response->entry_count; i++) {
    struct limine_memmap_entry* entry = memmap_request.response->entries[i];
    printf("   entry %d: base = 0x%llx, length = 0x%llx, type = %s\n", i, entry->base, entry->length, types[entry->type]);
  }
  asm("cli");
  for (;;)
    asm("hlt");
}
