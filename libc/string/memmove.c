/*====----------- memmove.c - string.h memmove implementation ------------====*\
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

#include <string.h>

#include <stdlib.h> /* malloc(), free() */

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
void* memmove(void *dest, const void *src, size_t n)
{
	if (dest < src) {
		/* memcpy copies forward */
		return memcpy(dest, src, n);
	}

	void *buf = malloc(n);

	memcpy(buf, src, n);
	memcpy(dest, buf, n);
	free(buf);

        return dest;
}
