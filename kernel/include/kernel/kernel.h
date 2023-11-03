/*====------------------ kernel.h - Kernel header file -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _KERNEL_H
#define _KERNEL_H

#define MAX_EXCEPTION_DEPTH 3

__attribute__((__noreturn__))
void panic(char *msg);

void k_except(int vec, char *nmon, char *name, int has_error, int abort);

#endif /* _KERNEL_H */