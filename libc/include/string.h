/*====--------- string.h - Standard string operations header -------------====*\
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

#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

__BEGIN_DECLS

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict dest, const void* __restrict src, size_t size);
void* memmove(void*, const void*, size_t);
void* memset(void* dest, int value, size_t n);
char* strcat(char*, const char*);
int strcmp(const char *str1, const char *str2);
size_t strlen(const char* str);

__END_DECLS

#endif /* _STRING_H */
