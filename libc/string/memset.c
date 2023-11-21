/*====------------ memset.c - string.h memset implementation -------------====*\
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

/**
 * Implementation heavily based on the Aulavik memcpy() implementation. There
 * are plenty of opportunities for optimization in this implementation, many
 * of which are mentioned in memcpy(). Any further improvement done to one
 * should be done to the other.
 */

#include <string.h>

#include <stdint.h> /* uint8_t */

/**
 * Copies the value stored in value to the first n bytes of memory starting at
 * the address pointed to by dest
 *
 * @param dest The address to begin writing to
 * @param value The value to write to memory
 * @param n The amount of bytes to write
 * @return A pointer to the start of the written region
 */
void* memset(void *dest, int value, size_t n)
{
	/* dereferencing a void pointer is impossible */
	register uint8_t *dest_byte = (uint8_t *) dest;

	register size_t l_size = sizeof(long);

	/*
	 * fast algorithm. we should only use this if n is large enough for
	 * this to be an improvement over the simple algorithm (four times the
	 * size of a long).
	 */
	if (n >= l_size * 4) {

		/*
		 * simple/slow copy to the destination until it is aligned to
		 * the long size, allowing for faster copying.
		 *
		 * throughout this implementation, n is decremented to be used
		 * to represent the amount of remaining bytes
		 */
		while (((uintptr_t) dest_byte & (l_size - 1))) {
			*dest_byte++ = value;
			n--;
		}

		/* long-sized (four/eight byte) chunks */
		long *dest_long = (long *) dest_byte;

		/*
		 * manually unrolled loop to copy four long-sized chunks
		 * at a time for as long as there are enough bytes remaining
		 */
		while (n >= l_size * 4) {
			dest_long[0] = value;
			dest_long[1] = value;
			dest_long[2] = value;
			dest_long[3] = value;
			dest_long += 4;
			n -= l_size * 4;
		}

		/*
		 * copy the leftover long-sized chunks that did not fit in
		 * chunks of four in the above loop
		 */
		while (n >= l_size) {
			*dest_long++ = value;
			n -= l_size;
		}

		dest_byte = (uint8_t *) dest_long;
	} /* fall-through to simple algorithm if leftover data exists */

	/*
	 * simple/fallback algorithm. we need to use this if n is so small that
	 * this method is faster than the more advanced algorithm, as it has a
	 * larger overhead.
	 *
	 * additionally, the faster algorithm relies on this to clean up any
	 * remaining bytes that were not able to be completed in bulk.
	 */
	while (n--)
		*dest_byte++ = value;

	return dest;
}
