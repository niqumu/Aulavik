/*====------------------- idt.h - IDT implementation ---------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/idt/idt.h>

#include <kernel/driver/ports.h>
#include <kernel/gdt/gdt.h>

/* defined in _idt.asm */
extern void *except_isr_stubs[];
extern void *irq_isr_stubs[];

/* idt itself */
uint64_t idt[IDT_MAX_ENTRIES];

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
	/* exceptions */
	for (uint8_t vector = 0; vector < 32; vector++) {
		idt[vector] = idt_create_descriptor((uintptr_t) except_isr_stubs[vector],
			FLAG_PRESENT | FLAG_PRIVILEGE_0 | FLAG_GATE_TYPE_TRAP);
	}

	/* irqs */
	for (uint8_t vector = 0; vector < 2; vector++) {
		idt[vector + 100] = idt_create_descriptor((uintptr_t) irq_isr_stubs[vector],
		        FLAG_PRESENT | FLAG_PRIVILEGE_0 | FLAG_GATE_TYPE_INT);
	}

	load_idt((IDT_MAX_ENTRIES * DESCRIPTOR_SIZE) - 1, (uintptr_t) &idt[0]);

	uint8_t a1 = port_inb(PIC_MASTER_DATA);
	uint8_t a2 = port_inb(PIC_SLAVE_DATA);

	port_outb(PIC_MASTER_COMMAND, 0x01 | 0x10);
	port_wait();
	port_outb(PIC_SLAVE_COMMAND, 0x01 | 0x10);
	port_wait();
	port_outb(PIC_MASTER_DATA, PIC_OFFSET);
	port_wait();
	port_outb(PIC_SLAVE_DATA, PIC_OFFSET);
	port_wait();
	port_outb(PIC_MASTER_DATA, 0x04);
	port_wait();
	port_outb(PIC_SLAVE_DATA, 0x02);
	port_wait();

	port_outb(PIC_MASTER_DATA, 0x01);
	port_wait();
	port_outb(PIC_SLAVE_DATA, 0x01);
	port_wait();

	port_outb(PIC_MASTER_DATA, a1);
	port_outb(PIC_SLAVE_DATA, a2);
}