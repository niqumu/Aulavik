/*====--------------- idt_handlers.c - Interrupt handlers ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/interrupts/idt.h>

#include <kernel/driver/ata.h>
#include <kernel/driver/keyboard.h>
#include <kernel/driver/mouse.h>
#include <kernel/driver/ports.h>
#include <kernel/kernel.h>
#include <kernel/syscalls/syscalls.h>
#include <kernel/terminal.h>
#include <kernel/task/scheduler.h>
#include <kernel/rainier/rainier.h>
#include "kernel/logger.h"

/* -----------------------------------
 *  Exceptions
 * ----------------------------------- */

int exception_depth = 0;

static void idt_handle_reserved(int vec)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(0, "RESERVED", "Unimplemented - triggered "
				"vector stored in error", vec, true);
	exception_depth--;
}

/* division error, no error code, no abort */
__attribute__((unused)) void idt_handle_vec0(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(0, "#DE", "Division Error", 0, false);
	exception_depth--;
}

/* debug, no error code, no abort */
__attribute__((unused)) void idt_handle_vec1(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(1, "#DB", "Debug", 0, false);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec2(uint32_t error) // TODO not an exception
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(2, "NMI", "Non-maskable Interrupt", error, false);
	exception_depth--;
}

/* breakpoint, no error, no abort */
__attribute__((unused)) void idt_handle_vec3(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(3, "#BP", "Breakpoint", 0, false);
	exception_depth--;
}

/* overflow, no error, no abort */
__attribute__((unused)) void idt_handle_vec4(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(4, "#OF", "Overflow", 0, false);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec5(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(5, "#BR", "Bound Range Exeeded", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec6(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(6, "#UD", "Invalid Opcode", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec7(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(7, "#NM", "Device Not Available", error, 0);
	exception_depth--;
}

/* double fault, error code, abort */
__attribute__((unused)) void idt_handle_vec8(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(8, "#DF", "Double Fault", error, true);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec9(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(9, "#CS", "Coprocessor Segment Overrun", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec10(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(10, "#TS", "Invalid TSS", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec11(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(11, "#NP", "Segment Not Present", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec12(uint32_t error) 
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(12, "SS", "Stack-Segment Fault", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec13(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(13, "#GP", "General Protection Fault", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec14(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(14, "#PF", "Page Fault", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec15(uint32_t error)
{
	idt_handle_reserved(15);
}

/* Floating-Point exception, no error, no abort */
__attribute__((unused)) void idt_handle_vec16(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(16, "#MF", "x87 Floating-Point Exception", 0, false);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec17(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(17, "#AC", "Alignment Check", error, 0);
	exception_depth--;
}

/* machine check, no error, abort */
__attribute__((unused)) void idt_handle_vec18(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(18, "#MC", "Machine Check", 0, true);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec19(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(19, "#XM/XF", "SIMD Floating-Point Exception", 0, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec20(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(20, "#VE", "Virtualization Exception", 0, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec21(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(21, "#CP", "Control Protection Exception", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec22()
{
	idt_handle_reserved(22);
}

__attribute__((unused)) void idt_handle_vec23()
{
	idt_handle_reserved(23);
}


__attribute__((unused)) void idt_handle_vec24()
{
	idt_handle_reserved(24);
}


__attribute__((unused)) void idt_handle_vec25()
{
	idt_handle_reserved(25);
}


__attribute__((unused)) void idt_handle_vec26()
{
	idt_handle_reserved(27);
}


__attribute__((unused)) void idt_handle_vec27()
{
	idt_handle_reserved(27);
}

__attribute__((unused)) void idt_handle_vec28(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(28, "#HV", "Hypervisor Injection Exception", 0, false);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec29(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(29, "#VC", "VMM Communication Exception", error, false);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec30(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(29, "#SX", "Security Exception", error, false);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec31(uint32_t error)
{
	idt_handle_reserved(31);
}

/* -----------------------------------
 *  IRQs
 * ----------------------------------- */

/* clock */
__attribute__((unused)) void idt_handle_vec80()
{
	struct cpu_state old_state = {0};

#ifdef RAINIER_DEBUGGING_ELEMENTS
	rainier_tick();
#endif /* RAINIER_DEBUGGING_ELEMENTS */

	terminal_tick();
	ata_tick();
	mouse_tick();
	port_pic_eoi();

	if (scheduler_initialized())
		scheduler_switch_next(old_state);
}

/* keyboard */
__attribute__((unused)) void idt_handle_vec81()
{
	keyboard_handle_press();
}

__attribute__((unused)) void idt_handle_vec82()
{
}

__attribute__((unused)) void idt_handle_vec83()
{
}

__attribute__((unused)) void idt_handle_vec84()
{
}

__attribute__((unused)) void idt_handle_vec85()
{
}

__attribute__((unused)) void idt_handle_vec86()
{
}

__attribute__((unused)) void idt_handle_vec87()
{
}

__attribute__((unused)) void idt_handle_vec88()
{
}

__attribute__((unused)) void idt_handle_vec89()
{
}

__attribute__((unused)) void idt_handle_vec90()
{
}

__attribute__((unused)) void idt_handle_vec91()
{
}

/* mouse */
__attribute__((unused)) void idt_handle_vec92()
{
	mouse_handle_interrupt();
}

__attribute__((unused)) void idt_handle_vec93()
{
}

__attribute__((unused)) void idt_handle_vec94()
{
}

__attribute__((unused)) void idt_handle_vec95()
{
}


/* -----------------------------------
 *  syscall
 * ----------------------------------- */
__attribute__((unused)) void idt_handle_vec128(void)
{
	uint32_t syscall;
	uint32_t edi, esi, edx;

	asm volatile ("mov %%eax, %0" : "=g" (syscall) :: "eax");
	asm volatile ("mov %%edi, %0" : "=g" (edi) :: "edi");
	asm volatile ("mov %%esi, %0" : "=g" (esi) :: "esi");
	asm volatile ("mov %%edx, %0" : "=g" (edx) :: "edx");

	syscalls_handle(syscall, edi, esi, edx);
}