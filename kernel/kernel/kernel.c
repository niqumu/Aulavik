/*====------------------ kernel.h - Main kernel entry --------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/kernel.h>

#include <stdint.h>
#include <stdio.h> /* printf() */

#include <driver/tty.h> /* terminal_init(); */
#include <driver/serial.h>

#include <kernel/logger.h>
#include <kernel/idt.h> /* idt_init() */
#include <kernel/gdt.h> /* gdt_init(); */

void k_except(int vec, char *nmon, char *name, int has_error, int abort)
{
	uint32_t error = 0;

	k_print("");
	k_error("Exception caught!");
	k_print("----------------------------------------");
	k_print("Exception: %x %s (%s)", vec, nmon, name);

	if (has_error) {
		/* TODO get from stack */
	}

	k_print("error: %x, abort: %d", error, abort);
	k_print("Stack dump:");

	uint32_t *stack_bottom;
	asm volatile ("mov %%ebp, %0" : "=g" (stack_bottom));

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

	if (abort)
		panic("Abort");
}

__attribute__((__noreturn__))
void panic(char *msg)
{
	k_print("\nKernel panic: %s", msg);
	while (1)
		asm volatile ("cli; hlt");
}

void kernel_main(void)
{
	terminal_init();
	k_ok("Initialized terminal");
	gdt_init();
	k_ok("Initialized GDT");
	idt_init();
	k_ok("Initialized IDT");

	k_ok("Loading drivers...");
	serial_init(PORT_COM_1, BAUD_38400);
	k_ok("Loaded serial driver!");

	k_print("\nKernel ready!");
	k_print("%dkb of memory present\n", *((uint64_t*) 0x413));
}