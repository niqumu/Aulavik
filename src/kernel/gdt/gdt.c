/*====------------------- gdt.h - GDT implementation ---------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h> /* uint8_t, uint32_t, uint64_t */

#include <kernel/gdt.h>
#include <kernel/logger.h>

/* gdt itself */
uint64_t *gdt;

uint64_t create_descriptor(uint32_t base, uint32_t limit,
			   uint8_t flags, uint8_t access)
{
	if (limit > 0xfffff)
		k_error("bad segment descriptor: limit %x is too big!", limit);

	if (flags > 0xf)
		k_error("bad segment descriptor: flags %x are too big!", flags);

	uint64_t dest;
	dest  = limit & 0x00ffff;
	dest |= (((uint64_t) limit) & 0xff0000) << 32;

	dest |= (base & 0x00ffffff) << 16;
	dest |= (((uint64_t) base) & 0xff000000) << 56;

	dest |= ((uint64_t) access) << 40;
	dest |= ((uint64_t) flags) << 52;
	return dest;
}

static void create_default_descriptors(void) {

	uint8_t access;
	uint8_t flags = FLAG_GRANULARITY | FLAG_SIZE;

	/* create blank first entry */
	gdt[0] = 0;

	/* create kernel code segment */
	access = (ACCESS_PRESENT | ACCESS_PRIVILEGE_0 | ACCESS_TYPE |
		  ACCESS_EXECUTABLE | ACCESS_READWRITE);
	gdt[1] = create_descriptor(0, 0xfffff, flags, access);

	/* create kernel data segment */
	access = (ACCESS_PRESENT | ACCESS_PRIVILEGE_0 | ACCESS_TYPE |
		  ACCESS_READWRITE);
	gdt[2] = create_descriptor(0, 0xfffff, flags, access);

	/* create user code segment */
	access = (ACCESS_PRESENT | ACCESS_PRIVILEGE_3 | ACCESS_TYPE |
		  ACCESS_EXECUTABLE | ACCESS_READWRITE);
	gdt[3] = create_descriptor(0, 0xfffff, flags, access);

	/* create user data segment */
	access = (ACCESS_PRESENT | ACCESS_PRIVILEGE_3 | ACCESS_TYPE |
		  ACCESS_READWRITE);
	gdt[4] = create_descriptor(0, 0xfffff, flags, access);

	/* create task sate segment */
	access = (ACCESS_PRESENT | ACCESS_PRIVILEGE_0 | ACCESS_EXECUTABLE |
		  ACCESS_ACCESSED);
	gdt[5] = create_descriptor(0, 0xfffff, flags, access);
}

void gdt_init(void)
{
	create_default_descriptors();

	load_gdt((GDT_ENTRIES * DESCRIPTOR_SIZE) - 1, (uint32_t) gdt);
	reload_registers();
}