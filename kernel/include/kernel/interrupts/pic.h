/*====------------ pic.h - Programmable Interrupt Controller -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef INTERRUPTS_PIC_H
#define INTERRUPTS_PIC_H

#include <stdbool.h>
#include <stdint.h>

#define PIC_OFFSET      80

/* tests if the provided irq is masked (disabled) on the pic */
bool pic_test_mask(uint8_t irq);

/* masks/unmasks (disables/enables respectively) the provided irq on the pic */
void pic_set_mask(uint8_t irq, bool masked);

void pic_init(void);

#endif /* INTERRUPTS_PIC_H */
