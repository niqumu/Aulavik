/*====--------------- idt_handlers.c - Interrupt handlers ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/idt/idt.h>

#include <kernel/driver/keyboard.h>

#include <kernel/logger.h>
#include <kernel/kernel.h>
#include <kernel/terminal.h>
#include <kernel/driver/ports.h>
#include "kernel/driver/ide.h"

/* -----------------------------------
 *  Exceptions
 * ----------------------------------- */

int exception_depth = 0;

static void idt_handle_reserved(int vec)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(0, "RESERVED", "Unimplemented", 0, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec0(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(0, "#DE", "Division Error", 1, 1);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec1(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(1, "#DB", "Debug", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec2(uint32_t error) // TODO not an exception
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(2, "NMI", "Non-maskable Interrupt", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec3(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(3, "#BP", "Breakpoint", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec4(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(4, "#OF", "Overflow", error, 0);
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

__attribute__((unused)) void idt_handle_vec8(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(8, "#DF", "Double Fault", 1, 1);
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

__attribute__((unused)) void idt_handle_vec16(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(16, "#MF", "x87 Floating-Point Exception", 0, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec17(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(17, "#AC", "Alignment Check", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec18(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(18, "#MC", "Machine Check", 0, 1);
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

__attribute__((unused)) void idt_handle_vec22(uint32_t error)
{
	idt_handle_reserved(22);
}

__attribute__((unused)) void idt_handle_vec23(uint32_t error)
{
	idt_handle_reserved(23);
}


__attribute__((unused)) void idt_handle_vec24(uint32_t error)
{
	idt_handle_reserved(24);
}


__attribute__((unused)) void idt_handle_vec25(uint32_t error)
{
	idt_handle_reserved(25);
}


__attribute__((unused)) void idt_handle_vec26(uint32_t error)
{
	idt_handle_reserved(27);
}


__attribute__((unused)) void idt_handle_vec27(uint32_t error)
{
	idt_handle_reserved(27);
}

__attribute__((unused)) void idt_handle_vec28(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(28, "#HV", "Hypervisor Injection Exception", 0, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec29(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(29, "#VC", "VMM Communication Exception", error, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec30(uint32_t error)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(29, "#SX", "Security Exception", error, 0);
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
	terminal_tick();
	ide_tick();
	port_pic_eoi();
}

/* keyboard */
__attribute__((unused)) void idt_handle_vec81()
{
	keyboard_handle_press();
}


/* -----------------------------------
 *  syscall
 * ----------------------------------- */
__attribute__((unused)) void idt_handle_vec128()
{
	k_ok("Syscall");
}