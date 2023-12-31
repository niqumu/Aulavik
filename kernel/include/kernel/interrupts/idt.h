/*====------------------- idt.h - IDT helper header ----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef INTERRUPTS_IDT_H
#define INTERRUPTS_IDT_H

#include <stdint.h> /* uint8_t, uint16_t, uint32_t, uint64_t */

#include <kernel/task/process.h>

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

/* -----------------------------------------
 *  Exceptions
 * --------------------------------------- */

/* division error, no error code, no abort */
__attribute__((unused)) void idt_handle_vec0(void);
/* debug, no error code, no abort */
__attribute__((unused)) void idt_handle_vec1(void);
/* NMI */
__attribute__((unused)) void idt_handle_vec2(uint32_t error);
/* breakpoint, no error, no abort */
__attribute__((unused)) void idt_handle_vec3(void);
/* overflow, no error, no abort */
__attribute__((unused)) void idt_handle_vec4(void);
__attribute__((unused)) void idt_handle_vec5(uint32_t error);
__attribute__((unused)) void idt_handle_vec6(uint32_t error);
__attribute__((unused)) void idt_handle_vec7(uint32_t error);
/* double fault, error code, abort */
__attribute__((unused)) void idt_handle_vec8(uint32_t error);
__attribute__((unused)) void idt_handle_vec9(uint32_t error);
__attribute__((unused)) void idt_handle_vec10(uint32_t error);
__attribute__((unused)) void idt_handle_vec11(uint32_t error);
__attribute__((unused)) void idt_handle_vec12(uint32_t error);
__attribute__((unused)) void idt_handle_vec13(uint32_t error);
__attribute__((unused)) void idt_handle_vec14(uint32_t error);
__attribute__((unused)) void idt_handle_vec15(uint32_t error);
/* Floating-Point exception, no error, no abort */
__attribute__((unused)) void idt_handle_vec16(void);
__attribute__((unused)) void idt_handle_vec17(uint32_t error);
/* machine check, no error, abort */
__attribute__((unused)) void idt_handle_vec18(void);
__attribute__((unused)) void idt_handle_vec19(uint32_t error);
__attribute__((unused)) void idt_handle_vec20(uint32_t error);
__attribute__((unused)) void idt_handle_vec21(uint32_t error);
__attribute__((unused)) void idt_handle_vec22();
__attribute__((unused)) void idt_handle_vec23();
__attribute__((unused)) void idt_handle_vec24();
__attribute__((unused)) void idt_handle_vec25();
__attribute__((unused)) void idt_handle_vec26();
__attribute__((unused)) void idt_handle_vec27();
__attribute__((unused)) void idt_handle_vec28(uint32_t error);
__attribute__((unused)) void idt_handle_vec29(uint32_t error);
__attribute__((unused)) void idt_handle_vec30(uint32_t error);
__attribute__((unused)) void idt_handle_vec31(uint32_t error);

/* -----------------------------------------
 *  IRQs
 * --------------------------------------- */

__attribute__((unused)) void idt_handle_vec80(struct cpu_state state);
__attribute__((unused)) void idt_handle_vec81();
__attribute__((unused)) void idt_handle_vec82();
__attribute__((unused)) void idt_handle_vec83();
__attribute__((unused)) void idt_handle_vec84();
__attribute__((unused)) void idt_handle_vec85();
__attribute__((unused)) void idt_handle_vec86();
__attribute__((unused)) void idt_handle_vec87();
__attribute__((unused)) void idt_handle_vec88();
__attribute__((unused)) void idt_handle_vec89();
__attribute__((unused)) void idt_handle_vec90();
__attribute__((unused)) void idt_handle_vec91();
__attribute__((unused)) void idt_handle_vec92();
__attribute__((unused)) void idt_handle_vec93();
__attribute__((unused)) void idt_handle_vec94();
__attribute__((unused)) void idt_handle_vec95();


/* -----------------------------------------
 *  Syscall
 * --------------------------------------- */
__attribute__((unused)) void idt_handle_vec128(void);

__attribute__((unused))
void idt_handle_fault_error(uint16_t vec);

__attribute__((unused))
void idt_handle_fault(uint16_t vec);

__attribute__((unused))
void idt_handle_abort_error(uint16_t vec);

__attribute__((unused))
void idt_handle_abort(uint16_t vec);

/* creates and returns a gate descriptor with the provided data */
uint64_t idt_create_descriptor(uintptr_t base, uint8_t flags);

/* enables ints and load an idt given the size and location. impl: _idt.asm */
extern void load_idt(uint32_t limit, uintptr_t base);

/* creates and load an idt */
void idt_init(void);

#endif /* INTERRUPTS_IDT_H */
