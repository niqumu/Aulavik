/*====----------------- ports.c - hardware ports driver ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <driver/ports.h>

#include <stdint.h> /* uint8_t, uint16_t */

void port_outb(uint16_t port, uint8_t data)
{
	asm volatile ("outb %0, %1" :: "a" (data), "Nd" (port));
}

uint8_t port_inb(uint16_t port)
{
	uint8_t response;
	asm volatile ("inb %1, %0" : "=a" (response) : "Nd" (port));
	return response;
}