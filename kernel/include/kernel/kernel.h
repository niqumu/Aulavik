/*====------------------ kernel.h - Kernel header file -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h>

#include <kernel/multiboot.h>

#ifndef _KERNEL_H
#define _KERNEL_H

/* how many times exceptions can recurse before a panic is called */
#ifndef MAX_EXCEPTION_DEPTH
#define MAX_EXCEPTION_DEPTH 3
#endif

/* directly compatible with the memory info format used by multiboot */
typedef struct multiboot_memory_map {
	uint32_t size;
//	uint32_t base_addr_low;
//	uint32_t base_addr_high;
//	uint32_t length_low;
//	uint32_t length_high;
	uint64_t address;
	uint64_t length;
	uint16_t type;
} mb_memory_block_t;

extern multiboot_info_t *mb_info;
extern mb_memory_block_t *mb_memory_map;
extern uint32_t mb_memory_map_size;

__attribute__((__noreturn__))
void panic(char *msg);

void k_except(int vec, char *nmon, char *name, uint32_t has_error, int abort);

#endif /* _KERNEL_H */