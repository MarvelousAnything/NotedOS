#include "kernel/print.h"
#include "kernel/serial.h"
#include "kernel/vga.h"
#include "kernel/screen.h"
#include "multiboot2.h"

_Noreturn void kernel_main(unsigned long magic, unsigned long addr) {
  init_serial();
  init_vga();
  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
    send_string("Invalid magic number: ");
    send_hex_long(magic);
    write_serial('\n');
    goto halt;
  }

  if (addr & 7) {
    send_string("Unaligned MBI!\n");
    goto halt;
  }

  printk("Hello World!\n");
  struct multiboot_tag *tag;
  for (tag = (struct multiboot_tag *)(addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag +
                                      ((tag->size + 7) & ~7))) {
    printk("Tag 0x%x Size 0x%x\n", tag->type, tag->size);
    struct multiboot_tag_framebuffer *fb_tag;
    switch (tag->type) {
    case MULTIBOOT_TAG_TYPE_CMDLINE:
      printk("cmdline = %s\n", ((struct multiboot_tag_string *)tag)->string);
      break;
    case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
      printk("boot loader = %s\n",
             ((struct multiboot_tag_string *)tag)->string);
      break;
    case MULTIBOOT_TAG_TYPE_MODULE:
      printk("module at 0x%x%x-0x%x%x. cmdline = %s\n",
             ((struct multiboot_tag_module *)tag)->mod_start,
             ((struct multiboot_tag_module *)tag)->mod_end,
             ((struct multiboot_tag_module *)tag)->cmdline);
      break;
    case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
      printk("mem_lower = %uKB, mem_upper = %uKB\n",
             ((struct multiboot_tag_basic_meminfo *)tag)->mem_lower,
             ((struct multiboot_tag_basic_meminfo *)tag)->mem_upper);
      break;
    case MULTIBOOT_TAG_TYPE_BOOTDEV:
      printk("Boot device 0x%x,%u,%u\n",
             ((struct multiboot_tag_bootdev *)tag)->biosdev,
             ((struct multiboot_tag_bootdev *)tag)->slice,
             ((struct multiboot_tag_bootdev *)tag)->part);
      break;
    case MULTIBOOT_TAG_TYPE_MMAP: {
      multiboot_memory_map_t *mmap;
      send_string("mmap\n");
      for (mmap = ((struct multiboot_tag_mmap *)tag)->entries;
           (multiboot_uint8_t *)mmap < (multiboot_uint8_t *)tag + tag->size;
           mmap = (multiboot_memory_map_t *)((unsigned long)mmap +
                                             ((struct multiboot_tag_mmap *)tag)
                                                 ->entry_size)) {
        printk(" base_addr = 0x%x%x,"
               " length = 0x%x%x, type = 0x%x\n",
               (unsigned)(mmap->addr >> 32),
               (unsigned)(mmap->addr & 0xffffffff), (unsigned)(mmap->len >> 32),
               (unsigned)(mmap->len & 0xffffffff), (unsigned)mmap->type);
      }
    }; break;

    case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
      fb_tag = (struct multiboot_tag_framebuffer *)tag;
      void *fb = (void *)(unsigned long)fb_tag->common.framebuffer_addr;
      multiboot_uint32_t fb_color;
      send_string("addr = ");
      send_hex_long(fb_tag->common.framebuffer_addr);
      send_string(" type = ");
      send_hex(fb_tag->common.framebuffer_type);
      write_serial('\n');
      send_string("width = ");
      send_hex(fb_tag->common.framebuffer_width);
      send_string(" height = ");
      send_hex(fb_tag->common.framebuffer_height);
      send_string(" pitch = ");
      send_hex(fb_tag->common.framebuffer_pitch);
      send_string(" bpp = ");
      send_hex(fb_tag->common.framebuffer_bpp);
      send_string(" reserved = ");
      send_hex(fb_tag->common.reserved);
      write_serial('\n');
      screen.addr = fb_tag->common.framebuffer_addr;
      screen.bpp = fb_tag->common.framebuffer_bpp;
      screen.pitch = fb_tag->common.framebuffer_pitch;
      screen.height = fb_tag->common.framebuffer_height;
      screen.width = fb_tag->common.framebuffer_width;

      // init_vga(fb_tag->common.framebuffer_addr);
      switch (fb_tag->common.framebuffer_type) {
      case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
        fb_color = ((1 << fb_tag->framebuffer_red_mask_size) - 1)
                   << fb_tag->framebuffer_red_field_position;
        send_string("framebuffer color = ");
        send_hex(fb_color);
        write_serial('\n');
        break;
      }
      for (int i = 0; i < fb_tag->common.framebuffer_width &&
                      i < fb_tag->common.framebuffer_height;
           i++) {
        switch (fb_tag->common.framebuffer_bpp) {
        case 32: {
          multiboot_uint32_t *pixel =
              fb + fb_tag->common.framebuffer_pitch * i + 4 * i;
          *pixel = fb_color;
        } break;
        }
      }
      break;
    }
    clear_screen(0xff0000);
    draw_rectangle(0, 0, screen.width, 50, 0x00ff00);
    char *str = "Once upon a midnight dreary, whilst I pondered weak and weary.";
    char *str2 = "Welcome to NotedOS";
    draw_string_scaled((screen.width / 2) - (text_width(str, 1, 1) / 2), 17, str, 0x00, 1, 1);
    draw_string_scaled((screen.width / 2) - (text_width(str2, 1, 2) / 2), 70, str2, 0xeeffee, 1, 2);
    
  }
halt:
  while (1)
    asm("hlt");
}
