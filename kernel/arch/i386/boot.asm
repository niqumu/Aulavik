; Declare constants to build the multiboot header
; 	https://wiki.osdev.org/Multiboot
;
; The multiboot header is required to provide metadata to GRUB, and to allow
;   GRUB to recognize us in the first place.
FLAG_ALIGN      equ    	001b
FLAG_MEMINFO    equ     010b
FLAG_GRAPHICS   equ     100b
MBFLAGS         equ     FLAG_ALIGN | FLAG_MEMINFO | FLAG_GRAPHICS
MAGIC           equ     0x1badb002
CHECKSUM        equ     -(MAGIC + MBFLAGS)

; Build the multiboot header using the constants we declared. These are magic
;   values that are documented in the multiboot standard (see OS Dev Wiki link
;   in the constant declaration section). This marks the program as a kernel.
;
; The bootloader searches for this signature in the first 8 KiB of the kernel
;   file, aligned at a 32-bit boundary. We put the signature in its own section
;   so that the header can be forced to be within the first 8 KiB of the file.
section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM
	dd 0 ;
	dd 0 ;
	dd 0 ;
	dd 0 ;
	dd 0 ;
	dd 0
	dd 1920
	dd 1080
	dd 0

global stack_top

section .bss
align 16
stack_bottom:
	resb 16384		; 16 KiB
stack_top:

; The linker script specifies _start as the entry point to the kernel. As such,
;   the bootloader will jump to this position once the kernel has been loaded.
;
; Here, we are declaring _start as a function symbol with the given symbol size
section .text
global _start:function ( _start.end - _start)
_start:
	mov esp, stack_top

	; Push the multiboot info onto the stack and call premain to save these
	;   values in the kernel and perform sanity checks on them.
	;
	; This must be done before global constructors are called, as the eax
	;   and ebx registers, as well as the stack, will have been modified.
	push eax
	push ebx
	extern kernel_premain
        call kernel_premain

        ; Call the global constructors
        extern _init
        call _init

        ; Reset the stack to remove garbage data from stack traces
	xor ebp, ebp

	; Call the high-level kernel and pass execution to it. 
	extern kernel_main
	call kernel_main

	; If execution reaches this point, loop forever.
	;
	; Interrupts are disabled with the cli instruction.
	;
	; The hlt instruction will wait for the next interrupt to come. This
	;   should never happen, since they have been disabled.
	;
	; If the execution somehow continues (non-maskable interrupt), just
	;   jump to the current address forever.
	cli
	hlt
	jmp $

.end:
