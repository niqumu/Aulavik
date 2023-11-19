/*====--------------- stdlib.h - Standard utilities header ---------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that the relevant copyright
 * notice and permission notice shall be included in all copies or substantial
 * portions of this software and all documentation files.
 *
 * Refer to LICENSE for more information. These works are provided with
 * absolutely no warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stddef.h>

#include <sys/cdefs.h>

#ifndef _STDLIB_H
#define _STDLIB_H 1

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

_Noreturn void abort(void);

_Noreturn void exit(int status);

void* calloc(size_t size);

void* malloc(size_t size);

void* realloc(void *ptr, size_t size);

void free(void *ptr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _STDLIB_H */
