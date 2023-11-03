; *====-------- _idt.asm - Assembly IDT function implementations ----------====*
;
; This code is a part of the Aulavik project.
; Usage of these works is permitted provided that this instrument is retained
; with the works, so that any entity that uses the works is notified of this
; instrument. These works are provided without any warranty.
;
; *====--------------------------------------------------------------------====*

idtr:
	dw 0    ; idt limit
	dd 0    ; idt base

;isr_stub_0 0
;isr_stub_1 1
;isr_stub_2 2
;isr_stub_3 3
;isr_stub_4 4
;isr_stub_5 5
;isr_stub_6 6
;isr_stub_7 7
;isr_stub_8 8
;isr_stub_9 9
;isr_stub_10 10
;isr_stub_11 11
;isr_stub_12 12
;isr_stub_13 13
;isr_stub_14 14
;isr_stub_15 15
;isr_stub_16 16
;isr_stub_17 17
;isr_stub_18 18
;isr_stub_19 19
;isr_stub_20 20
;isr_stub_21 21
;isr_stub_22 22
;isr_stub_23 23
;isr_stub_24 24
;isr_stub_25 25
;isr_stub_26 26
;isr_stub_27 27
;isr_stub_28 28
;isr_stub_29 29
;isr_stub_30 30
;isr_stub_31 31

; -----------------------------------------------------------------------------
; external c function: exception handler
;
; global function defined and exposed in idt.h, impl in idt.c
; -----------------------------------------------------------------------------
extern exception_handler

%assign a 0
%rep 32
	isr_stub_%+a:
		call exception_handler
		iret            ; use iretq later when transitioning to 64-bit
%assign a a + 1
%endrep

global isr_stubs
isr_stubs:
	%assign i 0
	%rep 32
		;dd isr_err_stub_%+i ; use dq later when transitioning to 64-bit
		dd isr_stub_%+i;
	%assign i i + 1
	%endrep

; -----------------------------------------------------------------------------
; global function: load an idt given the size and location
; params: uint16_t limit, uintptr_t base
;
; global function defined and exposed in idt.h
; -----------------------------------------------------------------------------
global load_idt
load_idt:
	mov ax, [esp + 4]       ; uint16_t limit
	mov [idtr], ax
	mov eax, [esp + 8]      ; uintptr_t base
	mov [idtr + 2], eax
	lidt [idtr]             ; load idt
	sti                     ; re-enable interrupts as they're now handled
	ret