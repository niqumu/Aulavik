; *====--------- _loader.asm - Assembly program loader functions ----------====*
;
; This code is a part of the Aulavik project.
; Usage of these works is permitted provided that the relevant copyright
; notice and permission notice shall be included in all copies or substantial
; portions of this software and all documentation files.
;
; Refer to LICENSE for more information. These works are provided with
; absolutely no warranty.
;
; *====--------------------------------------------------------------------====*

; -----------------------------------------------------------------------------
; global function: switch to ring 3 and jump to a function
; params: void *callee
; -----------------------------------------------------------------------------
global loader_jump_ring3
loader_jump_ring3:

	mov ecx, [esp + 4]      ; void* callee

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
	push ecx                ; return address
	iret