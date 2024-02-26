OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS {
  . = 1M;
  /* Section for Multiboot2 header */
  .mb2_hdr : {
    KEEP(*(.mb2_hdr))
  }

  /* Kernel code starts at high memory address */
  /* . = 0xFFFFFFFF80000000; */

  /* Text section for executable code */
  .text : {
    *(.text*)
  }

  /* Read-only data section */
  .rodata : {
    *(.rodata*)
    *(.font_data*)
  }

  /* Initialized data section */
  .data : {
    *(.data*)
  }

  /* BSS section for uninitialized data */
  .bss : {
    *(COMMON)
    *(.bss*)
  }

}


