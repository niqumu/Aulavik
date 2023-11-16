/*====------------ memcpy.c - string.h memcpy implementation -------------====*\
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

/*
 * Reasonably fast implementation that copies data between pointers aligned
 * to the size of a long in blocks the size of a long. Should the pointers not
 * be aligned, or the bytes to copy are too small for the more advanced
 * algorithm to be practical (twice the size of a long), a naive traditional
 * implementation is used instead.
 *
 * If speed is highly desired when using this implementation, it is imperative
 * that the caller ensures that the two addresses are aligned to each other,
 * relative to the system long size. Note that the two addresses do not need to
 * directly be aligned to the system long size. The two addresses should be
 * misaligned by the same amount (if any).
 *
 * This implementation is not platform dependent and does not make any
 * assumptions or special optimizations for any platform or architecture.
 *
 * Room for improvement exists: using platform dependant tricks and SIMD
 * instructions can optimize it further. Using rep movsb is supposedly faster
 * on modern x86 chipsets, and should be implemented in the future.
 */

#include <string.h>

#include <stdint.h>

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
void* memcpy(void* restrict dest, const void* restrict src, size_t n)
{
	/* dereferencing a void pointer is impossible */
	register uint8_t *dest_byte = (uint8_t *) dest;
	register const uint8_t *src_byte = (const uint8_t *) src;

	register size_t l_size = sizeof(long);

	/*
	 * fast algorithm. we should only use this if n is large enough for
	 * this to be an improvement over the simple algorithm (twice the size
	 * of a long), and if the destination and source pointer are aligned
	 * to each other, relative to the long size.
	 *
	 * the addresses do not need to be aligned to the long size, only to
	 * each other relative to the long size; the two addresses must be
	 * misaligned by the same amount.
	 *
	 * supposedly, there are some situations where a compiler fails to
	 * recognize a modulo as being a candidate for optimization using an
	 * AND. for this reason, in this code, "AND (size - 1)" is used over
	 * "modulo size".
	 */
	if (n >= l_size * 2 && ((uintptr_t) src & (l_size - 1)) ==
			((uintptr_t) dest & (l_size - 1))) {

		/*
		 * simple/slow copy between the two pointers until they are
		 * aligned to the long size.
		 *
		 * throughout this implementation, n is decremented to be used
		 * to represent the amount of remaining bytes
		 */
		while (((uintptr_t) src_byte & (l_size - 1))) {
			*dest_byte++ = *src_byte++;
			n--;
		}

		/* long-sized (four/eight byte) chunks */
		long *dest_long = (long *) dest_byte;
		long const *src_long = (long const *) src_byte;

		/*
		 * manually unrolled loop to copy four long-sized chunks
		 * at a time for as long as there are enough bytes remaining
		 */
		while (n >= l_size * 4) {
			dest_long[0] = src_long[0];
			dest_long[1] = src_long[1];
			dest_long[2] = src_long[2];
			dest_long[3] = src_long[3];
			src_long += 4;
			dest_long += 4;
			n -= l_size * 4;
		}

		/*
		 * copy the leftover long-sized chunks that did not fit in
		 * chunks of four in the above loop
		 */
		while (n >= l_size) {
			*dest_long++ = *src_long++;
			n -= l_size;
		}

		dest_byte = (uint8_t *) dest_long;
		src_byte = (const uint8_t *) src_long;
	}

	/*
	 * simple/fallback algorithm. we need to use this if the source and
	 * destination addresses are not aligned, or if n is so small that this
	 * method is faster than the more advanced algorithm, as it has a
	 * larger overhead.
	 */
	while (n--)
		*dest_byte++ = *src_byte++;

	return dest;
}
