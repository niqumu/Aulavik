/*====------------ watchdog.c - Kernel exception/panic handler -----------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
 * Defines kernel methods k_except() and panic() for exception-handling and
 * kernel panics, respectively, as defined in kernel.h
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/kernel.h>

#include <stdint.h> /* uint_32_t */
#include <stdio.h> /* printf() */
#include <stdlib.h> /* abort() */

#include <kernel/driver/serial.h>
#include <kernel/logger.h>

#include <kernel/graphics/font_renderer.h>
#include <kernel/graphics/graphics.h>
#include <kernel/graphics/font.h>

void k_except(int vec, char *nmon, char *name, uint32_t error, bool abrt)
{
	k_print("");
	k_error("Exception caught!");
	k_print("----------------------------------------");
	k_print("Exception: %x %s (%s)", vec, nmon, name);

	k_print("error: %x, abort: %d", error, abrt);
	k_print("Stack dump:");

	/* pointer to the bottom of the stack is located in ebp */
	uint32_t *stack_bottom;
	asm volatile ("mov %%ebp, %0" : "=g" (stack_bottom));

	/* iterate over the stack until the end, or 10 entries */
	for (int i = 0; i < 10; i++) {
		if (!*stack_bottom)
			goto done_stack_trace;

		k_print("  %d: %x", i, *stack_bottom);
		stack_bottom += sizeof(uintptr_t);
	}
	k_print("     ...");
done_stack_trace:

	k_print("Register dump:");

	uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp;
	asm volatile ("mov %%eax, %0" : "=g" (eax));
	asm volatile ("mov %%ebx, %0" : "=g" (ebx));
	asm volatile ("mov %%ecx, %0" : "=g" (ecx));
	asm volatile ("mov %%edx, %0" : "=g" (edx));
	asm volatile ("mov %%esi, %0" : "=g" (esi));
	asm volatile ("mov %%edi, %0" : "=g" (edi));
	asm volatile ("mov %%esp, %0" : "=g" (esp));
	asm volatile ("mov %%ebp, %0" : "=g" (ebp));
	k_print("  eax: %x\t\taccumulator", eax);
	k_print("  ebx: %x\t\tbase", ebx);
	k_print("  ecx: %x\t\tcounter", ecx);
	k_print("  edx: %x\t\tdata", edx);
	k_print("  esi: %x\t\tsource", esi);
	k_print("  edi: %x\t\tdestination", edi);
	k_print("  esp: %x\t\tstack pointer", esp);
	k_print("  ebp: %x\t\tstack base pointer", ebp);

//	if (abrt)
		abort();
}

__attribute__((__noreturn__))
void panic(char *msg)
{
	/*
	 * printing directly then using serial, rather than calling
	 * k_print() (which does both), in case the system is in such an
	 * unstable state that serial usage will prevent the message from
	 * displaying on-screen
	 */
	printf("\nKernel panic: %s", msg);

#ifdef SERIAL_LOGGING_ENABLED
	serial_write_string(SERIAL_LOGGING_PORT, "Kernel panic: ");
	serial_write_string(SERIAL_LOGGING_PORT, msg);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
#endif /* SERIAL_LOGGING_ENABLED */

	fr_render_string_bg(*graphics_get_global_rctx(), 0, 0, "Kernel panic",
			 graphics_color_rgb(255, 255, 255), graphics_color_rgb(255, 0, 0));
	fr_render_string_bg(*graphics_get_global_rctx(), 0, font_height, msg,
	                    graphics_color_rgb(255, 255, 255), graphics_color_rgb(255, 0, 0));
	while (1)
		asm volatile ("cli; hlt");
}
