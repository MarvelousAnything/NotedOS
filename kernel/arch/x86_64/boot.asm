global _start
extern kernel_main

section .mb2_hdr
mb2_hdr_begin:
  dd 0xE85250D6 ; mb2 magic
  dd 0 ; protected mode
  dd (mb2_hdr_end - mb2_hdr_begin) ; length
  dd 0x100000000 - (0xE85250D6 + 0 + (mb2_hdr_end - mb2_hdr_begin)) ; checksum

align 8
mb2_framebuffer_req:
    dw 5
    dw 1
    dd (mb2_framebuffer_end - mb2_framebuffer_req)
    dd 0
    dd 0
    dd 0
mb2_framebuffer_end:
align 8
dw 0
dw 0
dd 8
mb2_hdr_end:

section .text
bits 32

_start:
    cli

    mov esp, stack_top
    mov ebp, stack_bottom

    push ebx
    push eax
    call kernel_main

section .bss
stack_bottom:
  resb 4096 * 16
stack_top:
