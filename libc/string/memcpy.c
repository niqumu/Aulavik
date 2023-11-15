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

#include <string.h>

#include <stdint.h>

/* todo https://stackoverflow.com/questions/51494787/optimization-in-memcpy-implementation */
void* memcpy(void* restrict dest_ptr, const void* restrict src_ptr, size_t n)
{
	unsigned char *pcDst = (unsigned char *) dest_ptr;
	unsigned char const *pcSrc = (unsigned char const *) src_ptr;

	if (n >= sizeof(long) * 2 &&
			((uintptr_t) src_ptr & (sizeof(long) - 1))
			== ((uintptr_t) dest_ptr & (sizeof(long) - 1))) {

		while (((uintptr_t)pcSrc & (sizeof(long) - 1)) != 0) {
			*pcDst++ = *pcSrc++;
			n--;
		}

		long *plDst = (long *)pcDst;
		long const *plSrc = (long const *)pcSrc;

		/* manually unroll the loop */
		while (n >= sizeof(long) * 4) {
			plDst[0] = plSrc[0];
			plDst[1] = plSrc[1];
			plDst[2] = plSrc[2];
			plDst[3] = plSrc[3];
			plSrc += 4;
			plDst += 4;
			n -= sizeof(long) * 4;
		}

		while (n >= sizeof(long)) {
			*plDst++ = *plSrc++;
			n -= sizeof(long);
		}

		pcDst = (unsigned char *)plDst;
		pcSrc = (unsigned char const *)plSrc;
	}

	while (n--) {
		*pcDst++ = *pcSrc++;
	}

	return dest_ptr;
}
