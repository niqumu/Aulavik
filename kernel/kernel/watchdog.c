/*====------------ watchdog.c - Kernel exception/panic handler -----------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/kernel.h>

#include <stdint.h> /* uint_32_t */

#include <kernel/logger.h>

void k_except(int vec, char *nmon, char *name, uint32_t error, int abort)
{
	k_print("");
	k_error("Exception caught!");
	k_print("----------------------------------------");
	k_print("Exception: %x %s (%s)", vec, nmon, name);

	k_print("error: %x, abort: %d", error, abort);
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

//	if (abort)
		panic("Abort");
}

__attribute__((__noreturn__))
void panic(char *msg)
{
	k_print("\nKernel panic: %s", msg);
	while (1)
		asm volatile ("cli; hlt");
}