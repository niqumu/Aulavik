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

; ---------------------------------------------------
;  Exceptions
; ---------------------------------------------------

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

isr_stub_0:
	pushad
	cld
	call idt_handle_vec0
	popad
	iret
isr_stub_1:
	pushad
	cld
	call idt_handle_vec1
	popad
	iret
isr_stub_2:
	pushad
	cld
	call idt_handle_vec2
        popad
	iret
isr_stub_3:
	pushad
	cld
	call idt_handle_vec3
        popad
	iret
isr_stub_4:
	pushad
	cld
	call idt_handle_vec4
        popad
	iret
isr_stub_5:
	pushad
	cld
	call idt_handle_vec5
        popad
	iret
isr_stub_6:
	pushad
	cld
	call idt_handle_vec6
        popad
	iret
isr_stub_7:
	pushad
	cld
	call idt_handle_vec7
        popad
	iret
isr_stub_8: 
	pushad
	cld
	call idt_handle_vec8
        popad
	iret
isr_stub_9: 
	pushad
	cld
	call idt_handle_vec9
        popad
	iret
isr_stub_10: 
	pushad
	cld
	call idt_handle_vec10
        popad
	iret
isr_stub_11:
	pushad
	cld
	call idt_handle_vec11
        popad
	iret
isr_stub_12:
	pushad
	cld
	call idt_handle_vec12
        popad
	iret
isr_stub_13:
	pushad
	cld
	call idt_handle_vec13
        popad
	iret
isr_stub_14:
	pushad
	cld
	call idt_handle_vec14
        popad
	iret
isr_stub_15:
	pushad
	cld
	call idt_handle_vec15
        popad
	iret
isr_stub_16:
	pushad
	cld
	call idt_handle_vec16
        popad
	iret
isr_stub_17:
	pushad
	cld
	call idt_handle_vec17
        popad
	iret
isr_stub_18:
	pushad
	cld
	call idt_handle_vec18
        popad
	iret
isr_stub_19:
	pushad
	cld
	call idt_handle_vec19
        popad
	iret
isr_stub_20:
	pushad
	cld
	call idt_handle_vec20
        popad
	iret
isr_stub_21:
	pushad
	cld
	call idt_handle_vec21
        popad
	iret
isr_stub_22:
	pushad
	cld
	call idt_handle_vec22
        popad
	iret
isr_stub_23:
	pushad
	cld
	call idt_handle_vec23
        popad
	iret
isr_stub_24:
	pushad
	cld
	call idt_handle_vec24
        popad
	iret
isr_stub_25:
	pushad
	cld
	call idt_handle_vec25
        popad
	iret
isr_stub_26:
	pushad
	cld
	call idt_handle_vec26
        popad
	iret
isr_stub_27:
	pushad
	cld
	call idt_handle_vec27
        popad
	iret
isr_stub_28:
	pushad
	cld
	call idt_handle_vec28
        popad
	iret
isr_stub_29:
	pushad
	cld
	call idt_handle_vec29
        popad
	iret
isr_stub_30:
	pushad
	cld
	call idt_handle_vec30
        popad
	iret
isr_stub_31:
	pushad
	cld
	call idt_handle_vec31
        popad
	iret

global except_isr_stubs
except_isr_stubs:
	%assign i 0
	%rep 32
		; use dq later when transitioning to 64-bit
		dd isr_stub_%+i;
	%assign i i + 1
	%endrep

; ---------------------------------------------------
;  IRQs
; ---------------------------------------------------

extern idt_handle_vec80
extern idt_handle_vec81
extern idt_handle_vec82
extern idt_handle_vec83
extern idt_handle_vec84
extern idt_handle_vec85
extern idt_handle_vec86
extern idt_handle_vec87
extern idt_handle_vec88
extern idt_handle_vec89
extern idt_handle_vec90
extern idt_handle_vec91
extern idt_handle_vec92
extern idt_handle_vec93
extern idt_handle_vec94
extern idt_handle_vec95

isr_stub_80:
	pushad
	cld
	call idt_handle_vec80
        popad
	iret
isr_stub_81:
	pushad
	cld
	call idt_handle_vec81
        popad
	iret
isr_stub_82:
        pushad
        cld
        call idt_handle_vec82
        popad
        iret
isr_stub_83:
        pushad
        cld
        call idt_handle_vec83
        popad
        iret
isr_stub_84:
        pushad
        cld
        call idt_handle_vec84
        popad
        iret
isr_stub_85:
        pushad
        cld
        call idt_handle_vec85
        popad
        iret
isr_stub_86:
        pushad
        cld
        call idt_handle_vec86
        popad
        iret
isr_stub_87:
        pushad
        cld
        call idt_handle_vec87
        popad
        iret
isr_stub_88:
        pushad
        cld
        call idt_handle_vec88
        popad
        iret
isr_stub_89:
        pushad
        cld
        call idt_handle_vec89
        popad
        iret
isr_stub_90:
        pushad
        cld
        call idt_handle_vec90
        popad
        iret
isr_stub_91:
        pushad
        cld
        call idt_handle_vec91
        popad
        iret
isr_stub_92:
        pushad
        cld
        call idt_handle_vec92
        popad
        iret
isr_stub_93:
        pushad
        cld
        call idt_handle_vec93
        popad
        iret
isr_stub_94:
        pushad
        cld
        call idt_handle_vec94
        popad
        iret
isr_stub_95:
        pushad
        cld
        call idt_handle_vec95
        popad
        iret

global irq_isr_stubs
irq_isr_stubs:
	dd isr_stub_80
        dd isr_stub_81
        dd isr_stub_82
        dd isr_stub_83
        dd isr_stub_84
        dd isr_stub_85
        dd isr_stub_86
        dd isr_stub_87
        dd isr_stub_88
        dd isr_stub_89
        dd isr_stub_90
        dd isr_stub_91
        dd isr_stub_92
        dd isr_stub_93
        dd isr_stub_94
        dd isr_stub_95

; ---------------------------------------------------
;  Syscall
; ---------------------------------------------------

extern idt_handle_vec128

isr_stub_128:
	pushad
	cld
	call idt_handle_vec128
        popad
	iret

global syscall_isr_stub
syscall_isr_stub:
	dd isr_stub_128