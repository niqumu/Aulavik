/*====----------------- paging.c - Paging implementation -----------------====*\
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

/* frames bitmap */
uint32_t *frame_bitmap;
uint32_t frame_count;

page_directory_t *page_directory = DIRECTORY_LOCATION;

/* mark the frame at the provided addr as used in the bitmap */
static void mark_frame_used(uintptr_t frame_address)
{
	uint32_t frame = frame_address / FRAME_SIZE;
	uint32_t index = INDEX_FROM_BIT(frame);
	frame_bitmap[index] |= (1 << OFFSET_FROM_BIT(frame));
}

/* mark the frame at the provided addr as free in the bitmap */
static void mark_frame_free(uintptr_t frame_address)
{
	uint32_t frame = frame_address / FRAME_SIZE;
	uint32_t index = INDEX_FROM_BIT(frame);
	frame_bitmap[index] &= ~(1 << OFFSET_FROM_BIT(frame));
}

/* tests if the frame at the provided addr is free in the bitmap */
static uint8_t is_frame_free(uintptr_t frame_address)
{
	uint32_t frame = frame_address / FRAME_SIZE;
	uint32_t index = INDEX_FROM_BIT(frame);
	return frame_bitmap[index] & (1 << OFFSET_FROM_BIT(frame));
}

/* returns the address of the first free frame, or 1 if none exist */
static uint32_t find_first_free_frame()
{
	for (uint32_t i = 0; i < INDEX_FROM_BIT(frame_count); i++) {

		/* if the entry is completely full, skip it */
		if (frame_bitmap[i] == 0xffffffff) {
			continue;
		}

		/* iterate over all bits in the entry */
		for (int bit = 0; bit < 32; bit++) {
			if (!(frame_bitmap[i] & (1 << bit))) {
				return i * bit * 32;
			}
		}
	}

	return 1; /* zero is a valid return value, one is impossible */
}

static void allocate_frame(uint32_t *page, uint8_t user, uint8_t writable)
{
	uint32_t first_free = find_first_free_frame();

	if (first_free == 1) {
		panic("Paging: No free frames");
		return;
	}

	mark_frame_used(first_free * FRAME_SIZE);
	*page = first_free;
	*page |= writable ? PAGE_FLAG_WRITABLE : 0;
	*page |= user ? PAGE_FLAG_USER_MODE : 0;
}

static void free_frame(uint32_t *page)
{
	mark_frame_free((uintptr_t) page);
	*page = 0;
}

static void enable_paging()
{
	asm volatile ("mov %0, %%cr3" :: "r" ((uint32_t) page_directory));
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0" : "=r" (cr0));
	asm volatile ("mov %0, %%cr0" :: "r" (cr0 | 0x80000000));
}

static void populate_table(page_table_t *table, uint32_t *address)
{
	for (int i = 0; i < 1024; i++) {
		table->entries[i] = (*address) | PAGE_FLAG_WRITABLE | 1;
		*address += FRAME_SIZE;
	}
}

void paging_init(void)
{
	/* the address of the next page table to create */
	page_table_t *page_table = TABLE_BASE_LOCATION;

	/* the address of the next frame to assign */
	uint32_t frame = 0;

	uint32_t memory = mb_info->mem_upper * 1000;
	frame_count = memory / FRAME_SIZE;
	uint32_t needed_tables = frame_count / 1024;

//	k_debug("Paging: %d bytes of ram, need %d frames, need %d tables",
//		memory, frame_count, needed_tables);

	/*
	 * TODO/FIXME:
	 *
	 * This is broken. I've spent days debugging it and I can't figure it
	 * out. Page tables do not generate correctly beyond the 0x2000000
	 * mark, even though the addresses being passed to the populate_table()
	 * function are correct, the entry is always zero when read back.
	 *
	 * I'm going to go insane if I try and spend any more time on this
	 * right now, but just do more investigation sooner rather than later
	 * and don't completely forget about this nasty bug.
	 *
	 * Sigh 11/6/23
	 */
	for (uint32_t i = 0; i < needed_tables; i++) {

		/* populate a page table */
		populate_table(page_table, &frame);

		/* put the page table in the directory */
		page_directory->entries[i] = (uint32_t) page_table;
		page_directory->entries[i] |= PAGE_FLAG_WRITABLE | 1;

//		k_ok("Page table %d: %x\t  table sample (entry 0): %x", i,
//		     page_directory->entries[i], page_table->entries[0]);

		page_table += 0x1000;
	}

	enable_paging();
//	k_ok("Paging enabled!");
}