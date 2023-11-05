/*====-------- memory_manager.h - Physical memory manager header ---------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stddef.h>
#include <stdint.h>

#include <kernel/kernel.h>

#ifndef _KERNEL_MEMORY_MANAGER_H
#define _KERNEL_MEMORY_MANAGER_H

/* where the start of our region to hand out memory from starts */
#define MEMORY_START 0x200000

#define BLOCK_MAGIC 0xbee

#define BLOCK_FLAG_AVAILABLE 0
#define BLOCK_FLAG_USED 1

typedef struct block_header  {
	uint16_t magic;
	uint64_t size;
	uint16_t used;
} __attribute__((packed)) block_header_t;

void memman_dump(void);

void* memman_allocate(size_t size);

void memman_free(void *ptr);

void memory_manager_init(void);

#endif /* _KERNEL_MEMORY_MANAGER_H */