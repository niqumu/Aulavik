/*====------------- serial.c - Serial driver implementation --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/serial.h>

#include <stdbool.h>
#include <stdint.h> /* uint8_t, uint16_t */

#include <kernel/driver/ports.h>
#include <kernel/logger.h>

static bool serial_write_ready(uint16_t port)
{
	return port_inb(port + REGISTER_PORT_LINE_STATUS) &
			  REGISTER_LINE_STATUS_THRE;
}

static bool serial_read_ready(uint16_t port)
{
	return port_inb(port + REGISTER_PORT_LINE_STATUS) &
			  REGISTER_LINE_STATUS_DR;
}

void serial_write(uint16_t port, uint8_t data)
{
	while (!serial_write_ready(port))
		;

	port_outb(port, data); /* todo ensure it's been initialized */
}

uint8_t serial_read(uint16_t port)
{
	while (!serial_read_ready(port))
		;

	return port_inb(port); /* todo ensure it's been initialized */
}

void serial_write_string(uint16_t port, char *data)
{
	while (*data)
		serial_write(port, *data++);
}

void serial_init(uint16_t port, enum baud baud)
{
//	uint64_t *data = (uint64_t*) 0x400;
//	for (int port = 1; port <= 4; port++)
//		k_debug("serial: COM %d is assigned to %x", port, *data++);

	port_outb(port + REGISTER_PORT_INT_ENABLE, 0);
	port_outb(port + REGISTER_PORT_LINE_CONTROL, 0b10000000);
	port_outb(port + REGISTER_PORT_BAUD_LSB, baud);
	port_outb(port + REGISTER_PORT_BAUD_MSB, 0);

	port_outb(port + REGISTER_PORT_LINE_CONTROL,
	            (LINE_PARITY_NONE | LINE_STOP_BITS_1));

	port_outb(port + REGISTER_PORT_INT_ID, 0xc7);

	port_outb(port + REGISTER_PORT_MODEM_CONTROL,
	            (REGISTER_MODEM_CONTROL_DTR | REGISTER_MODEM_CONTROL_RTS |
	            REGISTER_MODEM_CONTROL_OUT2));

	/* enable loopback mode to test the UART */
	port_outb(port + REGISTER_PORT_MODEM_CONTROL,
	            (REGISTER_MODEM_CONTROL_RTS | REGISTER_MODEM_CONTROL_OUT1 |
		    REGISTER_MODEM_CONTROL_OUT2 | REGISTER_MODEM_CONTROL_LOOP));

	port_outb(port, 0xae);

	uint16_t in = port_inb(port);

	if (in != 0xae) {
		k_error("serial: UART check failed!", port);
		k_error("serial: Sent %x, got %x", 0xae, in);
		return;
	}

	/* disable loopback mode for normal operation */
	port_outb(port + REGISTER_PORT_MODEM_CONTROL,
	           (REGISTER_MODEM_CONTROL_RTS | REGISTER_MODEM_CONTROL_OUT1));


	k_ok("serial: UART check passed (sent %x got %x)", 0xae, in);
	k_ok("serial: Set up serial port %x", port);

	k_ok("Loaded serial driver (port: %x, baud: %x)", port, baud);
}