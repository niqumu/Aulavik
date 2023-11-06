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
#include <kernel/logger.h>

/* defined in _idt.asm */
extern void *except_isr_stubs[];
extern void *irq_isr_stubs[];
extern void *syscall_isr_stub;

/* idt itself */
uint64_t idt[IDT_MAX_ENTRIES];

/* pic state */
// TODO move pic to its own file it's messy here
uint16_t pic_mask;

void idt_set_pic_mask(uint8_t irq, uint8_t masked)
{
	if (masked)
		pic_mask |= (1 << irq);
	else
		pic_mask &= ~(1 << irq);

	port_outb(PORT_PIC_MASTER_DATA, pic_mask & 0xff);
	port_outb(PORT_PIC_SLAVE_DATA, pic_mask >> 8);
}

static void idt_configure_pic(void)
{

	/* start pic initialization */
	port_outb(PORT_PIC_MASTER_COMMAND, 0x01 | 0x10);
	port_outb(PORT_PIC_SLAVE_COMMAND, 0x01 | 0x10);

	/* set offsets and set up master/slave cascade */
	port_outb(PORT_PIC_MASTER_DATA, PIC_OFFSET);
	port_outb(PORT_PIC_SLAVE_DATA, PIC_OFFSET + 8);
	port_outb(PORT_PIC_MASTER_DATA, 0x04);
	port_outb(PORT_PIC_SLAVE_DATA, 0x02);

	/* enable 8086 mode */
	port_outb(PORT_PIC_MASTER_DATA, 0x01);
	port_outb(PORT_PIC_SLAVE_DATA, 0x01);

	/* disable all irqs until an appropriate driver enables them */
	port_outb(PORT_PIC_MASTER_DATA, 0xff);
	port_outb(PORT_PIC_SLAVE_DATA, 0xff);
	pic_mask = 0xffff;
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
	/* exceptions */
	for (uint8_t vector = 0; vector < 32; vector++) {
		idt[vector] = idt_create_descriptor((uintptr_t) except_isr_stubs[vector],
			FLAG_PRESENT | FLAG_PRIVILEGE_0 | FLAG_GATE_TYPE_TRAP);
	}

	/* irqs */
	for (uint8_t vector = 0; vector < 2; vector++) {
		idt[vector + PIC_OFFSET] = idt_create_descriptor((uintptr_t) irq_isr_stubs[vector],
		        FLAG_PRESENT | FLAG_PRIVILEGE_0 | FLAG_GATE_TYPE_INT);
	}

	/* syscall */
	idt[128] = idt_create_descriptor((uintptr_t) syscall_isr_stub,
			 FLAG_PRESENT | FLAG_PRIVILEGE_0 | FLAG_GATE_TYPE_INT);

	idt_configure_pic();
	load_idt((IDT_MAX_ENTRIES * DESCRIPTOR_SIZE) - 1, (uintptr_t) &idt[0]);

	k_ok("Initialized IDT");
}