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

#define PIC_OFFSET              80

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

/* exceptions */
__attribute__((unused)) void idt_handle_vec0(uint32_t error);
__attribute__((unused)) void idt_handle_vec1(uint32_t error);
__attribute__((unused)) void idt_handle_vec2(uint32_t error);
__attribute__((unused)) void idt_handle_vec3(uint32_t error);
__attribute__((unused)) void idt_handle_vec4(uint32_t error);
__attribute__((unused)) void idt_handle_vec5(uint32_t error);
__attribute__((unused)) void idt_handle_vec6(uint32_t error);
__attribute__((unused)) void idt_handle_vec7(uint32_t error);
__attribute__((unused)) void idt_handle_vec8(uint32_t error);
__attribute__((unused)) void idt_handle_vec9(uint32_t error);
__attribute__((unused)) void idt_handle_vec10(uint32_t error);
__attribute__((unused)) void idt_handle_vec11(uint32_t error);
__attribute__((unused)) void idt_handle_vec12(uint32_t error);
__attribute__((unused)) void idt_handle_vec13(uint32_t error);
__attribute__((unused)) void idt_handle_vec14(uint32_t error);
__attribute__((unused)) void idt_handle_vec15(uint32_t error);
__attribute__((unused)) void idt_handle_vec16(uint32_t error);
__attribute__((unused)) void idt_handle_vec17(uint32_t error);
__attribute__((unused)) void idt_handle_vec18(uint32_t error);
__attribute__((unused)) void idt_handle_vec19(uint32_t error);
__attribute__((unused)) void idt_handle_vec20(uint32_t error);
__attribute__((unused)) void idt_handle_vec21(uint32_t error);
__attribute__((unused)) void idt_handle_vec22(uint32_t error);
__attribute__((unused)) void idt_handle_vec23(uint32_t error);
__attribute__((unused)) void idt_handle_vec24(uint32_t error);
__attribute__((unused)) void idt_handle_vec25(uint32_t error);
__attribute__((unused)) void idt_handle_vec26(uint32_t error);
__attribute__((unused)) void idt_handle_vec27(uint32_t error);
__attribute__((unused)) void idt_handle_vec28(uint32_t error);
__attribute__((unused)) void idt_handle_vec29(uint32_t error);
__attribute__((unused)) void idt_handle_vec30(uint32_t error);
__attribute__((unused)) void idt_handle_vec31(uint32_t error);

/* IRQs */
__attribute__((unused)) void idt_handle_vec80();
__attribute__((unused)) void idt_handle_vec81();

/* syscall */
__attribute__((unused)) void idt_handle_vec128();

__attribute__((unused))
void idt_handle_fault_error(uint16_t vec);

__attribute__((unused))
void idt_handle_fault(uint16_t vec);

__attribute__((unused))
void idt_handle_abort_error(uint16_t vec);

__attribute__((unused))
void idt_handle_abort(uint16_t vec);

void idt_set_pic_mask(uint8_t irq, uint8_t masked);

/* creates and returns a gate descriptor with the provided data */
uint64_t idt_create_descriptor(uintptr_t base, uint8_t flags);

/* enables ints and load an idt given the size and location. impl: _idt.asm */
extern void load_idt(uint32_t limit, uintptr_t base);

/* creates and load an idt */
void idt_init(void);

#endif /* _KERNEL_IDT_H */