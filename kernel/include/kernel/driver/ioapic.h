/*====-------------- ioapic.h - Intel I/O APIC driver header -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef DRIVER_IOAPIC_H
#define DRIVER_IOAPIC_H

#include <stdint.h> /* uint64_t */

#define IOAPICID        0
#define IOAPICVER       1
#define IOAPICARB       2
#define IOREDTBL(i)     ((i * 2) + 16);

#define FLAG_DELIVERY_MODE_FIXED     0b000
#define FLAG_DELIVERY_MODE_LOWEST    0b001
#define FLAG_DELIVERY_MODE_SMI       0b010
#define FLAG_DELIVERY_MODE_NMI       0b100
#define FLAG_DELIVERY_MODE_INIT      0b101
#define FLAG_DELIVERY_MODE_EXTINIT   0b111

#define FLAG_DESTINATION_PHYSICAL    0b0000
#define FLAG_DESTINATION_LOGICAL     0b1000

uint64_t ioapic_create_entry(uint8_t vector, uint8_t);

#endif /* DRIVER_IOAPIC_H */