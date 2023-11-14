/*====------------ pic.c - Programmable Interrupt Controller -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/interrupts/pic.h>

#include <kernel/driver/ports.h>

uint16_t pic_mask = 0xffff; /* all irqs masked until something changes */

/* tests if the provided irq is masked (disabled) on the pic */
bool pic_test_mask(uint8_t irq)
{
	return pic_mask & (1 << irq);
}

/* masks/unmasks (disables/enables respectively) the provided irq on the pic */
void pic_set_mask(uint8_t irq, bool masked)
{
	if (masked)
		pic_mask |= (1 << irq);
	else
		pic_mask &= ~(1 << irq);

	port_outb(PORT_PIC_MASTER_DATA, pic_mask & 0xff);
	port_outb(PORT_PIC_SLAVE_DATA, pic_mask >> 8);
}

void pic_init(void)
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

	port_outb(PORT_PIC_MASTER_DATA, pic_mask & 0xff);
	port_outb(PORT_PIC_SLAVE_DATA, pic_mask >> 8);

	/* enable the clock, no reason to ever have it off */
	pic_set_mask(0, false);

	/* enable the second pic to raise irqs to the first */
	pic_set_mask(2, false);
}
