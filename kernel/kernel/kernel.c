/*====------------------ kernel.h - Main kernel entry --------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h> /* printf() */

#include <driver/serial.h>
#include <kernel/logger.h>
#include <kernel/idt.h> /* idt_init() */
#include <kernel/gdt.h> /* gdt_init(); */
#include <kernel/tty.h> /* terminal_init(); */

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

	k_debug("About to divide by zero!");
	k_print("\tTest!");
	asm volatile ("div %0" :: "a" (0));
}