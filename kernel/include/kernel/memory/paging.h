/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h>

#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#define FRAME_SIZE              0x1000

#define DIRECTORY_LOCATION      ((void *) 0x9c000)
#define TABLE_BASE_LOCATION     ((void *) 0x9d000)

#define PAGE_FLAG_PRESENT       0b00000001
#define PAGE_FLAG_WRITABLE      0b00000010
#define PAGE_FLAG_USER_MODE     0b00000100
#define PAGE_FLAG_ACCESSED      0b00100000
#define PAGE_FLAG_DIRTY         0b01000000

/* an array of 1024 pointers to frames of physical memory */
typedef struct page_table {
	uintptr_t entries[1024];
} page_table_t;

/* an array of 1024 pointers to page tables */
typedef struct page_directory {
	uintptr_t entries[1024];
} page_directory_t;

void paging_init(void);

#ifdef __cplusplus
};
#endif

#endif /* _KERNEL_PAGING_H */