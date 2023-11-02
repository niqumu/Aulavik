/*====------------- logger.h - Header for the kernel logger --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _KERNEL_LOGGER_H
#define _KERNEL_LOGGER_H

void k_ok(const char* __restrict, ...);
void k_warn(const char* __restrict, ...);
void k_error(const char* __restrict, ...);

#endif /* _KERNEL_LOGGER_H */