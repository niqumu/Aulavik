/*====------------------- gdt.h - GDT implementation ---------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/gdt/gdt.h>

#include <aulavik/syscall.h>
#include <stdio.h>
#include <string.h>

#include <kernel/logger.h>
#include <kernel/gdt/tss.h>
#include "kernel/bin/shell.h"

/* gdt itself */
uint64_t gdt[GDT_MAX_ENTRIES];

/* tss */
struct tss tss = {0};

uint32_t isr_stack[128];

uint64_t* gdt_get_descriptor(uint8_t index)
{
	if (index > GDT_MAX_ENTRIES) {
		k_error("gdt: tried to get illegal descriptor %d", index);
		return 0;
	}

	return &gdt[index];
}

uint64_t gdt_create_descriptor(uint32_t base, uint32_t limit,
			       uint8_t flags, uint8_t access)
{
	if (limit > 0xfffff) /* sanity check, as the data type is wider */
		k_error("gdt: bad descriptor: limit %x is too big!", limit);

	if (flags > 0xf) /* sanity check, as the data type is wider */
		k_error("gdt: bad descriptor: flags %x are too big!", flags);

	uint64_t dest;
	dest  = limit & 0x0000ffff;
	dest |= (((uint64_t) limit) & 0xff0000) << 32;
	dest |= (((uint64_t) base) & 0x00ffffff) << 16;
	dest |= (((uint64_t) base) & 0xff000000) << 32;
	dest |= ((uint64_t) access) << 40;
	dest |= ((uint64_t) flags) << 52;
	return dest;
}

static void create_default_descriptors(void) {

	uint8_t access;
	uint8_t flags = FLAG_GRANULARITY | FLAG_SIZE;

	/* create blank first entry */
	gdt[GDT_SEGMENT_NULL] = 0;

	/* create kernel code segment */
	access = (ACCESS_PRESENT | ACCESS_PRIVILEGE_0 | ACCESS_TYPE |
		  ACCESS_EXECUTABLE | ACCESS_READWRITE);
	gdt[GDT_SEGMENT_KERNEL_CODE] = gdt_create_descriptor(
		0, 0xfffff, flags, access);

	/* create kernel data segment */
	access = (ACCESS_PRESENT | ACCESS_PRIVILEGE_0 | ACCESS_TYPE |
		  ACCESS_READWRITE);
	gdt[GDT_SEGMENT_KERNEL_DATA] = gdt_create_descriptor(
		0, 0xfffff, flags, access);

	/* create user code segment */
	access = (ACCESS_PRESENT | ACCESS_PRIVILEGE_3 | ACCESS_TYPE |
		  ACCESS_EXECUTABLE | ACCESS_READWRITE);
	gdt[GDT_SEGMENT_USER_CODE] = gdt_create_descriptor(
		0, 0xfffff, flags, access);

	/* create user data segment */
	access = (ACCESS_PRESENT | ACCESS_PRIVILEGE_3 | ACCESS_TYPE |
		  ACCESS_READWRITE);
	gdt[GDT_SEGMENT_USER_DATA] = gdt_create_descriptor(
		0, 0xfffff, flags, access);

	/* create task state segment */
	access = (SYS_ACCESS_PRESENT | SYS_ACCESS_PRIVILEGE_0 |
		  SYS_ACCESS_TSS_32_AVAIL);
	gdt[GDT_SEGMENT_TSS] = gdt_create_descriptor(
		(uint32_t) &tss, sizeof(struct tss), 0, access);
}

static void load_tss(void)
{
	memset(&tss, 0, sizeof(struct tss));

	tss.ss0 = GDT_SEGMENT_KERNEL_DATA * DESCRIPTOR_SIZE;
	tss.esp0 = (uint32_t) &isr_stack[0];

	// todo why doesn't this work??
	tss.eflags = (1 << 10); /* interrupt enable */

	asm volatile ("ltr %0" :: "a" (GDT_SEGMENT_TSS * DESCRIPTOR_SIZE));
}

void ring3_test(void)
{
//	char string[] = "a very long string that is sure to have issues! i "
//	                "can't think of a single reason why this would work "
//	                "here and not in ring 3...\n";
//	char string[] = "this is a very long string that likes to crash!";
	char string[] = "ring 3 syscall!";
	syscall_write(0, &string, sizeof(string));

	asm volatile ("1: jmp 1b");
}

void gdt_init(void)
{
	create_default_descriptors();
	load_gdt((GDT_MAX_ENTRIES * DESCRIPTOR_SIZE) - 1, (uint32_t) &gdt[0]);
	reload_registers();
	load_tss();

	k_ok("Initialized GDT");
}