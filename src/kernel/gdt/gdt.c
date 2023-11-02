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

/* gdt itself */
uint64_t *gdt;

// TODO validate limit as 20 bits and flags as 4 bits (need kernel error func)
uint64_t create_descriptor(uint32_t base, uint32_t limit,
				uint8_t flags, uint8_t access)
{
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
}

void initialize_gdt(void)
{
	create_default_descriptors();

	load_gdt((GDT_ENTRIES * DESCRIPTOR_SIZE) - 1, (uint32_t) gdt);
	reload_registers();
}