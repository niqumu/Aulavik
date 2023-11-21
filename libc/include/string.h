/*====--------- string.h - Standard string operations header -------------====*\
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

#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

__BEGIN_DECLS

/**
 * Compares the first n bytes of the memory region pointed to by ptr_a to the
 * first n bytes of the memory region pointed to by ptr_b.
 *
 * @param ptr_a The starting address of memory region a
 * @param ptr_b The starting address of memory region b
 * @param n The size, in bytes, of the region to compare
 * @return 1, 0, or -1, if the memory region pointed to by ptr_a is greater
 *      than, equal to, or less than the memory region pointed to by ptr_b,
 *      respectively.
 */
int memcmp(const void *ptr_a, const void *ptr_b, size_t n);

/**
 * Copies n bytes starting at the address pointed to by src to the address
 * pointed to by dest. The two areas must not overlap; src must be at least
 * n bytes from dest in both directions. If copying between two overlapping
 * regions is required, the memmove() function should to be used instead.
 *
 * @param dest The address to copy to
 * @param src The address to copy from
 * @param n The amount of bytes to copy
 * @return The value of dest_ptr
 */
void* memcpy(void* __restrict dest, const void* __restrict src, size_t n);

/**
 * Copies n bytes starting at the address pointed to by src to the address
 * pointed to by dest. Unlike memcpy(), the two areas may overlap. If the two
 * regions are guaranteed to never overlap, memcpy() should be used instead, as
 * it is certain to perform far faster.
 *
 * @param dest The address to copy to
 * @param src The address to copy from
 * @param n The amount of bytes to copy
 * @return The value of dest_ptr
 */
void* memmove(void* __restrict dest, const void* __restrict src, size_t n);

/**
 * Copies the value stored in value to the first n bytes of memory starting at
 * the address pointed to by dest
 *
 * @param dest The address to begin writing to
 * @param value The value to write to memory
 * @param n The amount of bytes to write
 * @return A pointer to the start of the written region
 */
void* memset(void *dest, int value, size_t n);

/**
 * Appends a copy of the string pointed to by src onto the end of the string
 * pointed to by dest. The terminating null-character of dest is overwritten.
 * The two strings should not overlap.
 * @param dest A pointer to a string to append onto
 * @param src A pointer to a string to append to dest
 * @return The value of dest
 */
char* strcat(char *dest, const char *src);

/**
 * Compares the string pointed to by str1 to the string pointed to by str2. If
 * the two strings are equal, the function will return zero. Otherwise, the
 * function will return the difference between the two characters where the
 * strings first differ.
 * @param str1 The string to compare against str2
 * @param str2 The string to compare against str1
 * @return Zero if the two strings are equal, or the difference between the
 *      characters where the strings first differ.
 */
int strcmp(const char *str1, const char *str2);

/**
 * Computes the length of the string pointed to by str, defined as the number
 * of characters prior to a null-terminator.
 * @param str A pointer to the string to inspect
 * @return The amount of characters in the string before a null-character
 */
size_t strlen(const char* str);

__END_DECLS

#endif /* !_STRING_H */
