/*====-------------- ports.c - hardware ports driver header --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef DRIVER_PORTS_H
#define DRIVER_PORTS_H

#include <stdint.h> /* uint8_t, uint16_t */

#define PORT_PIC_MASTER_COMMAND         0x20
#define PORT_PIC_MASTER_DATA            0x21
#define PORT_PIC_SLAVE_COMMAND          0xa0
#define PORT_PIC_SLAVE_DATA             0xa1

#define PORT_PS2_RW_DATA        0x60
#define PORT_PS2_READ_STATUS    0x64
#define PORT_PS2_WRITE_CMD      0x64

#define PIC_EOI    0x20

/* read/write 4 bytes to/from a port */
void port_outl(uint16_t port, uint32_t data);
uint32_t port_inl(uint16_t port);

/* read/write a byte to/from a port */
void port_outb(uint16_t port, uint8_t data);
uint8_t port_inb(uint16_t port);

void port_wait(void);

/* issues an end of interrupt command to the master pic */
void port_pic_eoi(void);

/* issues an end of interrupt command to both pics */
void port_pic_eoi_slave(void);

#endif /* DRIVER_PORTS_H */