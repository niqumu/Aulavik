/*====--------- string.h - Standard string operations header -------------====*\
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

#ifndef _STRING_H
#define _STRING_H

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict dest, const void* __restrict src, size_t size);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
char* strcat(char*, const char*);
int strcmp(const char *str1, const char *str2);
size_t strlen(const char*);

#ifdef __cplusplus
}
#endif

#endif /* _STRING_H */
