/*====------------------ kernel.h - Main kernel entry --------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h> /* printf() */

#include <kernel/logger.h>
#include <kernel/gdt.h> /* gdt_init(); */
#include <kernel/tty.h> /* terminal_init(); */

void kernel_main(void)
{
	terminal_init();
	k_ok("Initialized terminal");

	gdt_init();
	k_ok("Loaded GDT");

	k_ok("Kernel ready!\n");
}