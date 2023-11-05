/*====--------------- stdlib.h - Standard utilities header ---------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stddef.h>

#include <sys/cdefs.h>

#ifndef _STDLIB_H
#define _STDLIB_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);

void* malloc(size_t size);

void free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // _STDLIB_H
