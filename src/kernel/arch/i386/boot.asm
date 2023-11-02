; https://wiki.osdev.org/Bare_Bones_with_NASM

; Declare constants to build the multiboot header
; 	https://wiki.osdev.org/Multiboot
;
; The multiboot header is required to provide metadata to GRUB, and to allow
; GRUB to recognize us in the first place.
MBALIGN		equ    	1 << 0
MEMINFO		equ     1 << 1
MBFLAGS		equ     MBALIGN | MEMINFO
MAGIC		equ	0x1badb002
CHECKSUM	equ     -(MAGIC + MBFLAGS)

; Build the multiboot header using the constants we declared. These are magic
; values that are documented in the multiboot standard (see wiki.osdev.org link
; in the constant declaration section). This marks the program as a kernel.
;
; The bootloader searches for this signature in the first 8 KiB of the kernel
; file, aligned at a 32-bit boundary. We put the signature in its own section
; so that the header can be forced to be within the first 8 KiB of the file.
section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

section .bss
align 16
stack_bottom:
	resb 16384		; 16 KiB
stack_top:

; The linker script specifies _start as the entry point to the kernel. As such,
; the bootloader will jump to this position once the kernel has been loaded.
;
; Here, we are declaring _start as a function symbol with the given symbol size
section .text
global _start:function ( _start.end - _start)
_start:
	mov esp, stack_top
	
        ; Call the global constructors
        extern _init
        call _init

	; Call the high-level kernel and pass execution to it. 
	extern kernel_main
	call kernel_main

	; If execution reaches this point, loop forever.
	;
	; Interrupts are disabled with the cli instruction (clear interrupt
	; enable). This doesn't really make sense, since they should already
	; have been disabled by the  bootloader, but this is here just in case
	; they somehow get re-enabled by the kernel.
	;
	; The hlt instruction will wait for the next interrupt to come. This
	; should never happen, since they have been disabled.
	;
	; If the execution somehow continues (non-maskable interrupt), just
	; jump to the current address forever.
	cli
	hlt
	jmp $

.end:
