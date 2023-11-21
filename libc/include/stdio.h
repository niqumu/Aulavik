/*====------------- stdio.h - Standard I/O functions header --------------====*\
 *
 * This code is a part of the Aulavik project. The Aulavik project is licenced
 * under the MIT License.
 *
 * Usage of these works (including, yet not limited to, reuse, modification,
 * copying, distribution, and selling) is permitted provided that the relevant
 * copyright notice and permission notice (as specified in LICENSE) shall be
 * included in all copies or substantial portions of this software and all
 * documentation files.
 *
 * Refer to LICENSE for more information. These works are provided "AS IS" with
 * absolutely no warranty of any kind.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#include <stdarg.h> /* va_list */
#include <stdint.h> /* uint64_t */

typedef uint64_t FILE;

#define EOF (-1)

/*
 * Define macros for standard streams of type FILE *, associated with the
 * respective stream
 * @ ISO/IEC 9899:201 §7.21.1.3
 */
#define stdout  ((FILE *) 0)
#define stdin   ((FILE *) 1)
#define stderr  ((FILE *) 2)

__BEGIN_DECLS

int printf(const char* __restrict str, ...);
int putchar(int ch);
int puts(const char *str);
int sprintf(char* __restrict dest, const char* __restrict str, ...);
int vprintf(const char* __restrict str, va_list args);
int vsprintf(char* __restrict dest, const char* __restrict str, va_list args);

__END_DECLS

#endif /* _STDIO_H */
