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

#include <kernel/kernel.h>

int exception_depth = 0;

static void idt_handle_reserved(int vec)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(0, "RESERVED", "Unimplemented", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec0(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(0, "#DE", "Division Error", 1, 1); // TODO temporarily fatal
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec1(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(1, "#DB", "Debug", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec2(void) // TODO not an exception
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(2, "NMI", "Non-maskable Interrupt", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec3(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(3, "#BP", "Breakpoint", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec4(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(4, "#OF", "Overflow", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec5(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(5, "#BR", "Bound Range Exeeded", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec6(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(6, "#UD", "Invalid Opcode", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec7(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(7, "#NM", "Device Not Available", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec8(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(8, "#DF", "Double Fault", 1, 1);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec9(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(9, "#CS", "Coprocessor Segment Overrun", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec10(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(10, "#TS", "Invalid TSS", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec11(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(11, "#NP", "Segment Not Present", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec12(void) 
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(12, "SS", "Stack-Segment Fault", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec13(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(13, "#GP", "General Protection Fault", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec14(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(14, "#PF", "Page Fault", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec15(void)
{
	idt_handle_reserved(15);
}

__attribute__((unused)) void idt_handle_vec16(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(16, "#MF", "x87 Floating-Point Exception", 0, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec17(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(17, "#AC", "Alignment Check", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec18(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(18, "#MC", "Machine Check", 0, 1);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec19(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(19, "#XM/XF", "SIMD Floating-Point Exception", 0, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec20(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(20, "#VE", "Virtualization Exception", 0, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec21(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(21, "#CP", "Control Protection Exception", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec22(void)
{
	idt_handle_reserved(22);
}

__attribute__((unused)) void idt_handle_vec23(void)
{
	idt_handle_reserved(23);
}


__attribute__((unused)) void idt_handle_vec24(void)
{
	idt_handle_reserved(24);
}


__attribute__((unused)) void idt_handle_vec25(void)
{
	idt_handle_reserved(25);
}


__attribute__((unused)) void idt_handle_vec26(void)
{
	idt_handle_reserved(27);
}


__attribute__((unused)) void idt_handle_vec27(void)
{
	idt_handle_reserved(27);
}

__attribute__((unused)) void idt_handle_vec28(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(28, "#HV", "Hypervisor Injection Exception", 0, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec29(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(29, "#VC", "VMM Communication Exception", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec30(void)
{
	if (exception_depth++ >= MAX_EXCEPTION_DEPTH)
		panic("Exception depth exceeded MAX_EXCEPTION_DEPTH");
	k_except(29, "#SX", "Security Exception", 1, 0);
	exception_depth--;
}

__attribute__((unused)) void idt_handle_vec31(void)
{
	idt_handle_reserved(31);
}

/* IRQs */

__attribute__((unused)) void idt_handle_vec100(void)
{
}

__attribute__((unused)) void idt_handle_vec101(void)
{
	keyboard_handle_press();
}