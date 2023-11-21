/*====--------------- stdlib.h - Standard utilities header ---------------====*\
 *
 * This file is a part of the Aulavik project. The Aulavik project is free
 * software, licenced under the MIT License.
 *
 * Usage of these works (including, yet not limited to, reuse, modification,
 * copying, distribution, and selling) is permitted, provided that the relevant
 * copyright notice and permission notice (as specified in LICENSE) shall be
 * included in all copies or substantial portions of this software and all
 * documentation files.
 *
 * These works are provided "AS IS" with absolutely no warranty of any kind,
 * either expressed or implied.
 *
 * You should have received a copy of the MIT License alongside this software;
 * refer to LICENSE for information. If not, refer to https://mit-license.org.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

__BEGIN_DECLS

_Noreturn void abort(void);

_Noreturn void exit(int status);

void* __ensure_result_use calloc(size_t size);

void* __ensure_result_use malloc(size_t size);

void* __ensure_result_use realloc(void *ptr, size_t size);

void free(void *ptr);

__END_DECLS

#endif /* !_STDLIB_H */
