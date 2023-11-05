/*====------------------------ ps2.c - PS/2 driver -----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/ps2.h>

#include <stdint.h>

#include <kernel/driver/ports.h>
#include <kernel/logger.h>

void ps2_init(void)
{
	/* disable inputs and flush the buffer */
	port_outb(PS2_PORT_WRITE_CMD, PS2_CMD_DISABLE_FIRST_PORT);
	port_outb(PS2_PORT_WRITE_CMD, PS2_CMD_DISABLE_SECOND_PORT);
	port_inb(PS2_PORT_RW_DATA);

	/* controller self-test */
	port_outb(PS2_PORT_WRITE_CMD, PS2_CMD_CONTROLLER_SELF_TEST);
	if (port_inb(PS2_PORT_RW_DATA) != PS2_TEST_OK) {
		k_error("PS/2 controller tested bad");
		return;
	}

	/* port test */
//	port_outb(PS2_PORT_WRITE_CMD, PS2_CMD_TEST_FIRST_PORT);
//	if (port_inb(PS2_PORT_RW_DATA) != PS2_TEST_OK) {
//		k_error("PS/2 port #1 tested bad");
//		return;
//	}

	port_outb(PS2_PORT_WRITE_CMD, PS2_CMD_ENABLE_FIRST_PORT);
//	port_outb(PS2_PORT_WRITE_CMD, PS2_DEV_COMMAND_ECHO);

//	while (1) {
//		if (port_inb(PS2_PORT_RW_DATA) == PS2_MESSAGE_ECHO_RESPONSE)
//			break;
//	}
//
//	k_debug("got echo response");

	/* make sure ps/2 interrupts are enabled */
//	port_outb(PS2_PORT_WRITE_CMD, PS2_CMD_READ_BYTE_0);
//	uint8_t config = port_inb(PS2_PORT_RW_DATA);
//	port_outb(PS2_PORT_WRITE_CMD, PS2_CMD_WRITE_BYTE_0);
//	port_outb(PS2_PORT_WRITE_CMD, config | PS2_CONFIG_FIRST_INT_ENABLE);
}