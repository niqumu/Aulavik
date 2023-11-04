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

; -----------------------------------------------------------------------------
; external c function: exception handler
;
; global function defined and exposed in idt.h, impl in idt.c
; -----------------------------------------------------------------------------
extern idt_handle_vec0
extern idt_handle_vec1
extern idt_handle_vec2
extern idt_handle_vec3
extern idt_handle_vec4
extern idt_handle_vec5
extern idt_handle_vec6
extern idt_handle_vec7
extern idt_handle_vec8
extern idt_handle_vec9
extern idt_handle_vec10
extern idt_handle_vec11
extern idt_handle_vec12
extern idt_handle_vec13
extern idt_handle_vec14
extern idt_handle_vec15
extern idt_handle_vec16
extern idt_handle_vec17
extern idt_handle_vec18
extern idt_handle_vec19
extern idt_handle_vec20
extern idt_handle_vec21
extern idt_handle_vec22
extern idt_handle_vec23
extern idt_handle_vec24
extern idt_handle_vec25
extern idt_handle_vec26
extern idt_handle_vec27
extern idt_handle_vec28
extern idt_handle_vec29
extern idt_handle_vec30
extern idt_handle_vec31
extern idt_handle_vec100

isr_stub_0: call idt_handle_vec0
	iret
isr_stub_1: call idt_handle_vec1
	iret
isr_stub_2: call idt_handle_vec2
        iret
isr_stub_3: call idt_handle_vec3
        iret
isr_stub_4: call idt_handle_vec4
        iret
isr_stub_5: call idt_handle_vec5
        iret
isr_stub_6: call idt_handle_vec6
        iret
isr_stub_7: call idt_handle_vec7
        iret
isr_stub_8: call idt_handle_vec8
        iret
isr_stub_9: call idt_handle_vec9
        iret
isr_stub_10: call idt_handle_vec10
        iret
isr_stub_11: call idt_handle_vec11
        iret
isr_stub_12: call idt_handle_vec12
        iret
isr_stub_13: call idt_handle_vec13
        iret
isr_stub_14: call idt_handle_vec14
        iret
isr_stub_15: call idt_handle_vec15
        iret
isr_stub_16: call idt_handle_vec16
        iret
isr_stub_17: call idt_handle_vec17
        iret
isr_stub_18: call idt_handle_vec18
        iret
isr_stub_19: call idt_handle_vec19
        iret
isr_stub_20: call idt_handle_vec20
        iret
isr_stub_21: call idt_handle_vec21
        iret
isr_stub_22: call idt_handle_vec22
        iret
isr_stub_23: call idt_handle_vec23
        iret
isr_stub_24: call idt_handle_vec24
        iret
isr_stub_25: call idt_handle_vec25
        iret
isr_stub_26: call idt_handle_vec26
        iret
isr_stub_27: call idt_handle_vec27
        iret
isr_stub_28: call idt_handle_vec28
        iret
isr_stub_29: call idt_handle_vec29
        iret
isr_stub_30: call idt_handle_vec30
        iret
isr_stub_31: call idt_handle_vec31
        iret
isr_stub_100: call idt_handle_vec100
	iret

global isr_stubs
isr_stubs:
	%assign i 0
	%rep 32
		;dd isr_err_stub_%+i ; use dq later when transitioning to 64-bit
		dd isr_stub_%+i;
	%assign i i + 1
	%endrep
	dd isr_stub_100;