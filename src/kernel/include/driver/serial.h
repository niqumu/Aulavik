/*====----------------- serial.h - Serial driver header ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h> /* uint32_t */

#ifndef _DRIVER_SERIAL_H
#define _DRIVER_SERIAL_H

#define UART_RATE       115200

/* serial port addresses, not always guaranteed to exist! */
#define PORT_COM_1      0x3f8
#define PORT_COM_2      0x2f8
#define PORT_COM_3      0x3e8
#define PORT_COM_4      0x2e8

/* byte offsets */
#define REGISTER_PORT_DATA              0
#define REGISTER_PORT_INT_ENABLE        1
#define REGISTER_PORT_INT_ID            2
#define REGISTER_PORT_LINE_CONTROL      3
#define REGISTER_PORT_MODEM_CONTROL     4
#define REGISTER_PORT_LINE_STATUS       5
#define REGISTER_PORT_MODEM_STATUS      6
#define REGISTER_PORT_SCRATCH           7
/* port registers that are only accessible with the DLAB set */
#define REGISTER_PORT_BAUD_LSB          0
#define REGISTER_PORT_BAUD_MSB          1

#define REGISTER_INT_ENABLE_AVAIL       0b00000001
#define REGISTER_INT_ENABLE_EMPTY       0b00000010
#define REGISTER_INT_ENABLE_ERROR       0b00000100
#define REGISTER_INT_ENABLE_CHANGE      0b00001000

#define REGISTER_MODEM_CONTROL_DTR      0b00000001
#define REGISTER_MODEM_CONTROL_RTS      0b00000010
#define REGISTER_MODEM_CONTROL_OUT1     0b00000100
#define REGISTER_MODEM_CONTROL_OUT2     0b00001000
#define REGISTER_MODEM_CONTROL_LOOP     0b00010000

#define REGISTER_MODEM_STATUS_DCTS      0b00000001
#define REGISTER_MODEM_STATUS_DDSR      0b00000010
#define REGISTER_MODEM_STATUS_TERI      0b00000100
#define REGISTER_MODEM_STATUS_DDCD      0b00001000
#define REGISTER_MODEM_STATUS_CTS       0b00010000
#define REGISTER_MODEM_STATUS_DSR       0b00100000
#define REGISTER_MODEM_STATUS_RI        0b01000000
#define REGISTER_MODEM_STATUS_DCD       0b10000000

#define REGISTER_LINE_STATUS_DR         0b00000001
#define REGISTER_LINE_STATUS_OE         0b00000010
#define REGISTER_LINE_STATUS_PE         0b00000100
#define REGISTER_LINE_STATUS_FE         0b00001000
#define REGISTER_LINE_STATUS_BI         0b00010000
#define REGISTER_LINE_STATUS_THRE       0b00100000
#define REGISTER_LINE_STATUS_TEMT       0b01000000
#define REGISTER_LINE_STATUS_IE         0b10000000

/* line protocol */
#define LINE_CHAR_LENGTH_5      0b00
#define LINE_CHAR_LENGTH_6      0b01
#define LINE_CHAR_LENGTH_7      0b10
#define LINE_CHAR_LENGTH_8      0b11

#define LINE_STOP_BITS_1        0b0
#define LINE_STOP_BITS_2        0b1

#define LINE_PARITY_NONE        0b000
#define LINE_PARITY_ODD         0b001   /* data bits + parity bit odd */
#define LINE_PARITY_EVEN        0b011   /* data bits + parity bit even */
#define LINE_PARITY_MARK        0b101   /* parity bit 1 */
#define LINE_PARITY_SPACE       0b111   /* parity bit 0 */

enum baud {
	BAUD_115200 = 0b01,
	BAUD_57600 = 0b10,
	BAUD_38400 = 0b11
};

void serial_write(uint16_t port, uint8_t data);
uint8_t serial_read(uint16_t port);
void serial_write_string(uint16_t port, char *data);
char* serial_read_string(uint16_t port);

void serial_init(uint16_t port, enum baud);

#endif /* _DRIVER_SERIAL_H */