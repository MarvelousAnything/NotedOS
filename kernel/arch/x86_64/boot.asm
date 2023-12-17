global _start
extern kernel_main

section .mb2_hdr
mb2_hdr_begin:
  align 8
  dd 0xE85250D6
  dd 0
  dd (mb2_hdr_end - mb2_hdr_begin)
  dd -(0xE85250D6 + (mb2_hdr_end - mb2_hdr_begin))

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

_start:
    cli

    mov rsp, stack_top

	push rbx
	push rax

    call kernel_main

section .bss
resb 4096 * 16
stack_bottom:
stack_top:
