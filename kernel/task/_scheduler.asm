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
; params: struct cpu_state_out
;
; declared in scheduler.c
; -----------------------------------------------------------------------------
;global scheduler_load_state
;scheduler_load_state:
	popad

	;mov ax, (4 * 8) | 3     ; ring 3
        ;mov ds, ax
        ;mov es, ax
        ;mov fs, ax
        ;mov gs, ax
        ;mov ds, ax
        ;mov ax, (3 * 8) | 3
        ;mov cs, ax

	iret
