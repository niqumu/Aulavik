/*====------------------ kernel.h - Main kernel entry --------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h> /* printf() */

#include <kernel/gdt.h>
#include <kernel/tty.h>

void kernel_main(void)
{
	terminal_init();
	printf("[OK] Initialized terminal\n");

	initialize_gdt();
	printf("[OK] Loaded GDT\n");

	printf("[OK] Kernel ready!\n");
}