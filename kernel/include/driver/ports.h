/*====-------------- ports.c - hardware ports driver header --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h> /* uint8_t, uint16_t */

#ifndef _DRIVER_PORTS_H
#define _DRIVER_PORTS_H

void port_outb(uint16_t port, uint8_t data);
uint8_t port_inb(uint16_t port);

#endif /* _DRIVER_PORTS_H */