/*====----------- realloc.c - stdlib.h realloc implementation ------------====*\
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

#include <stdlib.h>

#include <string.h> /* memcpy() */

/**
 * Allocates size bytes of memory and returns a pointer to the start of the
 * allocated region. Data from ptr is copied over up to the lesser of the old
 * and new sizes. ptr is freed.
 *
 * If ptr is a null pointer, the function behaves the same as malloc().
 *
 * @param ptr The pointer to copy data from
 * @param size The new amount of bytes to allocate
 * @return A pointer to the start of the allocated block, or NULL
 */
void* __ensure_result_use realloc(void *ptr, size_t size)
{
	void *p = malloc(size);

	if (ptr != NULL && p != NULL) {
		memcpy(p, ptr, size);
		free(ptr);
	}

	return p;
}
