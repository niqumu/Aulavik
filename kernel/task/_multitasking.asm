; *====------ _multitasking.asm - Assembly taskswitching functions --------====*
;
; This code is a part of the Aulavik project.
; Usage of these works is permitted provided that this instrument is retained
; with the works, so that any entity that uses the works is notified of this
; instrument. These works are provided without any warranty.
;
; *====--------------------------------------------------------------------====*

extern current_task

; -----------------------------------------------------------------------------
; global function: switch to the provided task
; params: struct task *task
;
; global function defined and exposed in multitasking.h
; -----------------------------------------------------------------------------
global multitasking_switch
multitasking_switch:
