/*====------------------- gdt.h - GDT helper header ----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h> /* uint8_t, uint16_t, uint32_t, uint64_t */

/* segment descriptor access byte flags */
#define ACCESS_PRESENT	                0b10000000
#define ACCESS_PRIVILEGE_0	        0b00000000
#define ACCESS_PRIVILEGE_1	        0b00100000
#define ACCESS_PRIVILEGE_2	        0b01000000
#define ACCESS_PRIVILEGE_3	        0b01100000
#define ACCESS_TYPE		        0b00010000
#define ACCESS_EXECUTABLE	        0b00001000
#define ACCESS_DIRECTION	        0b00000100
#define ACCESS_CONFORMING	        0b00000100
#define ACCESS_READWRITE	        0b00000010
#define ACCESS_ACCESSED	                0b00000001

/* segment descriptor flags */
#define FLAG_GRANULARITY	        0b1000
#define FLAG_SIZE		        0b0100
#define FLAG_LONG		        0b0010

/* system segment descriptor access byte flags */
#define SYS_ACCESS_PRESENT	        0b10000000
#define SYS_ACCESS_PRIVILEGE_0          0b00000000
#define SYS_ACCESS_PRIVILEGE_1          0b00100000
#define SYS_ACCESS_PRIVILEGE_2          0b01000000
#define SYS_ACCESS_PRIVILEGE_3          0b01100000
#define SYS_ACCESS_LDT	                0b00000010
#define SYS_ACCESS_TSS_16_AVAIL         0b00000001
#define SYS_ACCESS_TSS_16_BUSY          0b00000011
#define SYS_ACCESS_TSS_32_AVAIL         0b00001001
#define SYS_ACCESS_TSS_32_BUSY          0b00001011
#define SYS_ACCESS_TSS_64_AVAIL         0b00001001
#define SYS_ACCESS_TSS_64_BUSY          0b00001011

/* null, kernel code and data, user code and data */
#define GDT_ENTRIES 5

/* the byte size of a single segment descriptor */
#define DESCRIPTOR_SIZE 64

#ifdef __cplusplus
extern "C" {
#endif

/* representation of a 48-bit GDT descriptor, used in 32-bit mode */
struct __attribute__((packed)) gdt_descriptor_32 {
	uint32_t base;	        /* the linear address of the GDT */
	uint16_t limit;	        /* the size of the table in bytes, minus one */
};

/* representation of an 80-bit GDT descriptor, used in 64-bit mode */
struct __attribute__((packed)) gdt_descriptor_64 {
	uint64_t base;	        /* the linear address of the GDT */
	uint16_t limit;	        /* the size of the table in bytes, minus one */
};

/* representation of a reconstructed segment descriptor */
struct __attribute__((packed)) segment {
	uint32_t base;
	uint32_t limit;         /* 20 bits */
	uint8_t flags;          /* 4 bits */
	uint8_t access;
};

/* representation of a reconstructed long-mode system segment descriptor */
struct __attribute__((packed)) long_sys_segment {
	uint64_t base;
	uint32_t limit;         /* 20 bits */
	uint8_t flags;          /* 4 bits */
	uint8_t access;
};

/* creates and returns a segment descriptor with the provided data */
uint64_t create_descriptor(uint32_t base, uint32_t limit,
			   uint8_t flags, uint8_t access);

/* load a gdt given the size and location. impl: _gdt.asm */
extern void load_gdt(uint32_t limit, uint32_t base);

/* reload the segment registers, required after loading gdt. impl: _gdt.asm */
extern void reload_registers(void);

/* creates and load a gdt */
void gdt_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _KERNEL_GDT_H */