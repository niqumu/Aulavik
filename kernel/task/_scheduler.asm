; *====---------------- _scheduler.asm - Task scheduler -------------------====*
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
; global function: load the provided cpu state
; params: struct cpu_state
;
; declared in scheduler.c
; -----------------------------------------------------------------------------
global scheduler_load_state
scheduler_load_state:
	mov edi, [esp + 4]
	mov esi, [esp + 8]
	mov ebp, [esp + 12]
	mov ebx, [esp + 20]
	mov edx, [esp + 24]
	mov ecx, [esp + 28]
	mov eax, [esp + 32]
	sti
	jmp [esp + 36]

