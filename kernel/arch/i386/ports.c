/*====----------------- ports.c - hardware ports driver ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ports.h>

#include <stdint.h> /* uint8_t, uint16_t */

void port_outl(uint16_t port, uint32_t data)
{
	asm volatile ("outl %0, %1" :: "a" (data), "Nd" (port) : "memory");
}

uint32_t port_inl(uint16_t port)
{
	uint32_t response;
	asm volatile ("inl %1, %0" : "=a" (response) : "Nd" (port) : "memory");
	return response;
}

void port_outb(uint16_t port, uint8_t data)
{
	asm volatile ("outb %0, %1" :: "a" (data), "Nd" (port) : "memory");
}

uint8_t port_inb(uint16_t port)
{
	uint8_t response;
	asm volatile ("inb %1, %0" : "=a" (response) : "Nd" (port) : "memory");
	return response;
}

void port_wait(void)
{
	port_outb(0x80, 0);
}

void port_pic_eoi(void)
{
	port_outb(PORT_PIC_MASTER_COMMAND, PORT_PIC_EOI);
}

void port_pic_eoi_slave(void)
{
	port_outb(PORT_PIC_MASTER_COMMAND, PORT_PIC_EOI);
	port_outb(PORT_PIC_SLAVE_COMMAND, PORT_PIC_EOI);
}