/*====---------------- keyboard.c - PS/2 keyboard driver -----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/keyboard.h>

#include <kernel/driver/ports.h>
#include <kernel/idt/idt.h>
#include <kernel/logger.h>

void keyboard_handle_press(void)
{
	uint8_t scan_code = port_inb(0x60);
	k_debug("press: %x", scan_code);
	port_pic_eoi();
}

void keyboard_init(void)
{
	/* unmask keyboard irqs on the pic */
	idt_set_pic_mask(1, 0);
}