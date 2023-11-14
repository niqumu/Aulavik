/*====------------------- mouse.c - PS/2 mouse driver --------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that the relevant copyright 
 * notice and permission notice shall be included in all copies or substantial 
 * portions of this software and all documentation files.
 * 
 * Refer to LICENSE for more information. These works are provided with 
 * absolutely no warranty.
 * 
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/mouse.h>

#include <kernel/rainier/rainier.h>

#include <kernel/driver/ports.h>
#include <kernel/driver/ps2.h>
#include <kernel/interrupts/pic.h>
#include "kernel/logger.h"

#define PACKET_LEFT     0b00000001
#define PACKET_RIGHT    0b00000010
#define PACKET_MIDDLE   0b00000100
#define PACKET_X_SIGN   0b00010000
#define PACKET_Y_SIGN   0b00100000

volatile bool mouse_waiting = false;
volatile bool mouse_synced = false;

int packet_part = 0;
uint32_t packet;

void mouse_process_packet()
{
	struct mouse_packet s_packet;

	s_packet.left_state = packet & PACKET_LEFT;
	s_packet.middle_state = packet & PACKET_MIDDLE;
	s_packet.right_state = packet & PACKET_RIGHT;

	int byte2 = ((int) packet & 0x00ff00) >> 8;
	int byte3 = ((int) packet & 0xff0000) >> 16;
	s_packet.delta_x = byte2 - (((int) packet << 4) & 0x100);
	s_packet.delta_y = byte3 - (((int) packet << 3) & 0x100);

	s_packet.delta_y *= -1; /* no idea why, but this is needed */

	rainier_process_mouse(s_packet);
}

void mouse_handle_interrupt(void)
{
	uint8_t data = port_inb(PORT_PS2_RW_DATA);

	if (!mouse_synced) {
		k_debug("mouse: %d: %d", packet_part, data);

		if (data == 0b1000) {
			packet_part = 0;
			mouse_synced = true;
			k_debug("mouse: synced packets");
		} else {
			port_pic_eoi_slave();
			return;
		}
	}

	packet |= data << (8 * packet_part);
	packet_part++;

	if (packet_part == 3) {
		mouse_process_packet();
		packet_part = 0;
		packet = 0;
	}

	port_pic_eoi_slave();
}

void mouse_tick(void)
{
	mouse_waiting = false;
}

void mouse_wait(void)
{
	mouse_waiting = true;

	while (mouse_waiting)
		;
}

void mouse_write(uint8_t data)
{
	mouse_wait();
	port_outb(PORT_PS2_WRITE_CMD, PS2_CMD_WRITE_NEXT_SECOND_IN);
	mouse_wait();
	port_outb(PORT_PS2_RW_DATA, data);
}

uint8_t mouse_read()
{
	mouse_wait();
	return port_inb(PORT_PS2_RW_DATA);
}

void mouse_init(void)
{
	uint8_t status;

	port_outb(PORT_PS2_WRITE_CMD, PS2_CMD_ENABLE_SECOND_PORT);

	/* enable interrupts */
	mouse_wait();
	port_outb(PORT_PS2_WRITE_CMD, 0x20);
	mouse_wait();
	status = (port_inb(PORT_PS2_RW_DATA) | 2);
	mouse_wait();
	port_outb(PORT_PS2_WRITE_CMD, 0x60);
	mouse_wait();
	port_outb(PORT_PS2_RW_DATA, status);

	/* tell the mouse to restore default settings */
	mouse_write(0xF6);
	mouse_read();

	/* enable data reporting */
	mouse_write(0xF4);
	mouse_read();

	/* unmask interrupts from the mouse */
	pic_set_mask(12, false);

	k_ok("Mouse driver setup");
}