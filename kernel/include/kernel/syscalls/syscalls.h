/*====---------------- syscall.h - Syscall handler header ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef KERNEL_SYSCALLS_H
#define KERNEL_SYSCALLS_H

#include <stdint.h>

void syscalls_handle(uint32_t eax, uint32_t edi, uint32_t esi, uint32_t edx);

#endif /* KERNEL_SYSCALLS_H */
