/*====------------------- idt.h - IDT helper header ----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h> /* uint8_t, uint16_t, uint32_t, uint64_t */

#define FLAG_PRESENT            0b10000000
#define FLAG_PRIVILEGE_0	0b00000000
#define FLAG_PRIVILEGE_1	0b00100000
#define FLAG_PRIVILEGE_2	0b01000000
#define FLAG_PRIVILEGE_3	0b01100000
#define FLAG_GATE_TYPE_INT      0b00001110
#define FLAG_GATE_TYPE_TRAP     0b00001111

#define IDT_MAX_ENTRIES 256

/* the byte size of a single gate descriptor */
#define DESCRIPTOR_SIZE 8

/* representation of a 48-bit IDT descriptor, used in 32-bit mode */
struct __attribute__((packed)) idt_descriptor_32 {
	uintptr_t base;	        /* the linear address of the IDT */
	uint16_t limit;	        /* the size of the table in bytes, minus one */
};

/* representation of an 80-bit IDT descriptor, used in 64-bit mode */
struct __attribute__((packed)) idt_descriptor_64 {
	uintptr_t base;	        /* the linear address of the IDT */
	uint16_t limit;	        /* the size of the table in bytes, minus one */
};

/* representation of a reconstructed gate descriptor, used in 32-bit mode */
struct __attribute__((packed)) gate {
	uintptr_t base;
	uint16_t segment_selector;
	uint8_t flags;
};

__attribute__((__noreturn__))
void exception_handler(void);

/* creates and returns a gate descriptor with the provided data */
uint64_t idt_create_descriptor(uintptr_t base, uint8_t flags);

/* enables ints and load an idt given the size and location. impl: _idt.asm */
extern void load_idt(uint32_t limit, uintptr_t base);

/* creates and load an idt */
void idt_init(void);

#endif /* _KERNEL_IDT_H */