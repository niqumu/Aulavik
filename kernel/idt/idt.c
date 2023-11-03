/*====------------------- idt.h - IDT implementation ---------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/idt.h>

#include <kernel/gdt.h>
#include <kernel/logger.h>

/* defined in _idt.asm */
extern void *isr_stubs[];

/* idt itself */
uint64_t idt[IDT_MAX_ENTRIES];

__attribute__((__noreturn__))
void exception_handler(void)
{
	k_print("");
	k_error("FATAL: exception thrown, system halt!");
	while (1)
		asm volatile ("cli; hlt");
}

uint64_t idt_create_descriptor(uintptr_t base, uint8_t flags)
{
	uint64_t dest;
	dest  = (((uint64_t) base) & 0xffff0000) << 32;
	dest |= base & 0x0000ffff;
	dest |= ((uint64_t) flags) << 40;
	dest |= (GDT_SEGMENT_KERNEL_CODE * DESCRIPTOR_SIZE) << 16;

	return dest;
}

void idt_init(void)
{
	for (uint8_t vector = 0; vector < 32; vector++) {
		idt[vector] = idt_create_descriptor((uintptr_t) isr_stubs[vector],
			FLAG_PRESENT | FLAG_PRIVILEGE_0 | FLAG_GATE_TYPE_TRAP);
	}

	load_idt((IDT_MAX_ENTRIES * DESCRIPTOR_SIZE) - 1, (uintptr_t) &idt[0]);
}