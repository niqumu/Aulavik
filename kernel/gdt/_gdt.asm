; *====-------- _gdt.asm - Assembly GDT function implementations ----------====*
;
; This code is a part of the Aulavik project.
; Usage of these works is permitted provided that this instrument is retained
; with the works, so that any entity that uses the works is notified of this
; instrument. These works are provided without any warranty.
;
; *====--------------------------------------------------------------------====*

gdtr:
	dw 0    ; gdt limit
	dd 0    ; gdt base

; -----------------------------------------------------------------------------
; global function: load a gdt given the size and location
; params: uint16_t limit, uintptr_t base
;
; global function defined and exposed in gdt.h
; -----------------------------------------------------------------------------
global load_gdt
load_gdt:
	mov ax, [esp + 4]       ; uint16_t limit
	mov [gdtr], ax
	mov eax, [esp + 8]      ; uintptr_t base
	mov [gdtr + 2], eax
	lgdt [gdtr]             ; load gdt
	ret

; -----------------------------------------------------------------------------
; global function: reload the segment registers, required after loading gdt
;
; global function defined and exposed in gdt.h
; -----------------------------------------------------------------------------
global reload_registers
reload_registers:
	jmp 0x08:.reload_cs     ; long jump to kernel code segment
				; loads kernel code segment into cs
.reload_cs:
	mov ax, 0x10            ; kernel data segment
	mov ds, ax
	mov es, ax              ; load kernel data segment into all segment
	mov fs, ax              ;   registers, except for cs
	mov gs, ax
	mov ss, ax
	ret

; -----------------------------------------------------------------------------
;
; -----------------------------------------------------------------------------
global gdt_jump_ring3
gdt_jump_ring3:
	mov ax, (4 * 8) | 3     ; ring 3
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov eax, esp
	push (4 * 8) | 3        ; data selector
	push eax
	pushf
	push (3 * 8) | 3        ; code selector
	push ring3_test         ; return address
	iret

extern ring3_test