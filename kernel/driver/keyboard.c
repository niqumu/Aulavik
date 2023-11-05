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
#include <kernel/logger.h>

void keyboard_handle_press()
{
	k_debug("press");
	port_outb(0x20, 0x20);
}