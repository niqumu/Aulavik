/*====--------- string.h - Standard string operations header -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
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
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
char* strcat(char*, const char*);
int strcmp(const char *str1, const char *str2);
size_t strlen(const char*);

#ifdef __cplusplus
}
#endif

#endif /* _STRING_H */
