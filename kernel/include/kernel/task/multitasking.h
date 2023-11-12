/*====-------------- multitasking.h - Task scheduler header --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef KERNEL_MULTITASKING_H
#define KERNEL_MULTITASKING_H

#include <kernel/task/process.h>

struct cpu_state multitasking_switch_next(struct cpu_state old_state);

void multitasking_init(void);

#endif /* KERNEL_MULTITASKING_H */
