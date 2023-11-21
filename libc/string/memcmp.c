/*====------------- memcmp.c - string.h memcmp implementation ------------====*\
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

/*
 * This implementation is designed so that the execution time of the function
 * is independent of the contents of the memory blocks, for security purposes.
 * Here, the memory blocks are traversed in their entirety regardless of when,
 * or if, a mismatching byte is found.
 *
 * The only factor that affects the execution time of the function is the size
 * of the region being compared. However, the function in its entirety is also
 * skipped should the two pointers point to the same region.
 */

#include <string.h>

#include <stdint.h> /* uint8_t */

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
int memcmp(const void *ptr_a, const void *ptr_b, size_t n)
{
	if (ptr_a == ptr_b)
		return 0;

        const uint8_t *a = (const uint8_t *) ptr_a;
        const uint8_t *b = (const uint8_t *) ptr_b;

	/* prevent the compiler from optimizing writes and exiting early */
	volatile uint8_t r = 0;

	for (size_t i = 0; i < n; i++) {
		r |= a[i] ^ b[i];
        }

        return (signed int) r;
}
