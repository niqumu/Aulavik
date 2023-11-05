/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/logger.h>
#include <kernel/memory_manager.h>

uint64_t memory_end = 0;
block_header_t *first_block;

void memman_dump(void)
{
	k_debug("Memory dump:");
	uint64_t address = MEMORY_START;
	for (int i = 0; address < memory_end; i++) {
		block_header_t *block = (block_header_t *) address;

		if (block->magic != BLOCK_MAGIC) return;

		k_print("block %d: address: %l, size: %l, used: %d", i,
			address, block->size, block->used);

		address += block->size;
	}
}

int memman_verify_header(uint64_t address)
{
	return ((block_header_t *) address)->magic == BLOCK_MAGIC;
}

block_header_t* memman_next_block(block_header_t *block)
{
	uint64_t header = ((uint64_t) block) + block->size;

	if (memman_verify_header((uint64_t) header)) {
		return (block_header_t *) header;
	}

	return 0;
}

block_header_t* memman_write_header(uint64_t address, uint64_t size)
{
	block_header_t *header = (block_header_t *) address;
	header->magic = BLOCK_MAGIC;
	header->size = size;
	header->used = BLOCK_FLAG_AVAILABLE;

	return header;
}

void* memman_allocate(uint64_t size)
{
	/* the block needs to be bigger than the requested size in order
	 *   to fit the header as well */
	uint64_t size_needed = size + sizeof(block_header_t);
	block_header_t *block = first_block;

	/* search forever until we find a free block or reach the end */
	while (1) {
		if (block->used == BLOCK_FLAG_AVAILABLE) {

			/* make sure the block is big enough */
			if (block->size < size_needed)
				continue;

			/* split the free block up, writing a header at
			 * the end of the memory we actually need */
			memman_write_header(((uint64_t) block) + size_needed,
			                    block->size - size_needed);

			/* write a header for the block being requested */
			memman_write_header((uint64_t) block, size_needed);

			block->used = BLOCK_FLAG_USED;

			/* pointer to the start of usable memory in block */
			return (void *) (((uint64_t) block) + sizeof(block_header_t));
		}

		block = memman_next_block(block);

		/* if the next block is invalid, we've reached the end */
		if (!block) {
			k_error("Failed to allocate %d bytes!", size_needed);
			return 0;
		}
	}
}

void memman_free(void *ptr)
{
	uint64_t block_addr = ((uint64_t) ptr) - sizeof(block_header_t);
	block_header_t *block = (block_header_t *) block_addr;

	if (!memman_verify_header(block_addr)) {
		k_error("free called on illegal address!");
		return;
	}

	block->used = BLOCK_FLAG_AVAILABLE;
}

void memory_manager_init(void)
{
	uint64_t free = 0;

	for (uint32_t i = 0; i < kernel_get_mb_memmap_size(); i++) {
		mb_memory_block_t mb_block = kernel_get_mb_memmap()[i];

		if (mb_block.type == MULTIBOOT_MEMORY_AVAILABLE) {
			if (!mb_block.address)
				continue;

			memory_end = mb_block.address + mb_block.length;
			free = memory_end - MEMORY_START;
			break;
		}
	}

	if (!free)
		panic("No memory");

	first_block = memman_write_header(MEMORY_START, free);
}