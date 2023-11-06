/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/memory/paging.h>

#include <kernel/kernel.h> /* kernel_get_mb_info() */
#include <kernel/logger.h>

page_directory_t *page_directory = DIRECTORY_LOCATION;

void write_cr3(uint32_t value)
{
	asm volatile ("mov %0, %%cr3" :: "r" (value));
}

void write_cr0(uint32_t value)
{
	asm volatile ("mov %0, %%cr0" :: "r" (value));
}

uint32_t read_cr0()
{
	uint32_t value;
	asm volatile ("mov %%cr0, %0" : "=r" (value));
	return value;
}

static void paging_fill_table(page_table_t *table, uint64_t *address)
{
	for (int i = 0; i < 1024; i++) {
		table->entries[i] = *address | PAGE_FLAG_WRITABLE | PAGE_FLAG_PRESENT;
		*address += FRAME_SIZE;
	}
}

void paging_init(void)
{
	/* the address of the next page table to create */
	page_table_t *page_table = TABLE_BASE_LOCATION;

	/* the address of the next frame to assign */
	uint64_t address = 0;

	uint32_t memory = kernel_get_mb_info()->mem_upper * 1000;
	uint32_t needed_frames = memory / 0x1000;
	uint32_t needed_tables = needed_frames / 1024;

	k_debug("%d bytes of ram, need %d frames, need %d tables", memory, needed_frames, needed_tables);

	for (uint32_t i = 0; i < needed_tables; i++) {

		/* populate a page table */
		paging_fill_table(page_table, &address);

		/* put the page table in the directory */
		page_directory->entries[i] = (uintptr_t) page_table;
		page_directory->entries[i] |= PAGE_FLAG_WRITABLE | PAGE_FLAG_PRESENT;

		page_table += 0x1000;
	}

	write_cr3((uint32_t) page_directory);
	write_cr0(read_cr0() | 0x80000000);
	k_ok("Paging enabled!");
}