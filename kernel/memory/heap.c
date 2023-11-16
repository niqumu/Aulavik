/*====---------------- heap.h - Basic heap implementation ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/logger.h>
#include <kernel/memory/heap.h>

uint64_t memory_end = 0;
block_header_t *first_block;

/* state */
bool heap_ready = false;

static int memman_verify_header(uint64_t address)
{
	return ((block_header_t *) address)->magic == BLOCK_MAGIC;
}

static block_header_t* memman_next_block(block_header_t *block)
{
	uint64_t header = ((uint64_t) block) + block->size;

	/* if the memory after this block isn't a valid block, we're at
	 *   the end of memory */
	if (memman_verify_header((uint64_t) header)) {
		return (block_header_t *) header;
	}

	return 0;
}

static block_header_t* memman_write_header(uint64_t address, uint64_t size)
{
	block_header_t *header = (block_header_t *) address;
	header->magic = BLOCK_MAGIC;
	header->size = size;
	header->used = BLOCK_FLAG_AVAILABLE;

	return header;
}

static void memman_merge_blocks(void)
{
	block_header_t *block = first_block;

	/* first iteration, search all blocks */
	while (1) {

		/* if the block is free, we want to merge it with all
		 *   free blocks immediately following it */
		if (block->used == BLOCK_FLAG_AVAILABLE) {
			block_header_t *block2 = block;

			/* second iteration, find free blocks immediately
			 *   following the current block */
			while (1) {
				if (block2->used == BLOCK_FLAG_USED) {

					/* merge all the free blocks */
					uint64_t b_addr = (uint64_t) block;
					uint64_t b2_addr = (uint64_t) block2;

					block->size = b2_addr - b_addr;
					break;
				}

				/* find the next, or stop if end of memory */
				if (!(block2 = memman_next_block(block2)))
					return;
			}

		}

		/* find the next, or stop if we reached the end of memory */
		if (!(block = memman_next_block(block)))
			return;
	}

}

void heap_dump(void)
{
	k_debug("Memory dump:");
	block_header_t *block = first_block;

	for (int i = 0; 1; i++) {
		k_print("block %d\taddress: %l     size: %l\t  used: %d",
			i, block, block->size, block->used);

		/* find the next, or stop if we reached the end of memory */
		if (!(block = memman_next_block(block)))
			return;
	}
}

void* heap_alloc(size_t size)
{
	/* the block needs to be bigger than the requested size in order
	 *   to fit the header as well */
	size_t size_needed = size + sizeof(block_header_t);
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

		/* find the next, or stop if we reached the end of memory */
		if (!(block = memman_next_block(block))) {
			k_error("Failed to allocate %d bytes!", size_needed);
			panic("Out of memory");
		}
	}
}

void heap_free(void *ptr)
{
	uint64_t block_addr = ((uint64_t) ptr) - sizeof(block_header_t);
	block_header_t *block = (block_header_t *) block_addr;

	/* if the memory address isn't the start of a block */
	if (!memman_verify_header(block_addr)) {
		k_error("free called on illegal address!");
		return;
	}

	block->used = BLOCK_FLAG_AVAILABLE;
	memman_merge_blocks();
}

void heap_init(void)
{
	uint64_t free = 0;

	/* find how big our usable memory is by reading multiboot info */
	for (uint32_t i = 0; i < mb_memory_map_size; i++) {
		mb_memory_block_t mb_block = mb_memory_map[i];

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

	/* create one big block to start with */
	first_block = memman_write_header(MEMORY_START, free);
	heap_ready = true;
}