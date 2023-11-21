/*====------------ malloc.c - stdlib.h malloc implementation -------------====*\
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

#ifdef __AULAVIK_LIBK
	#include <kernel/memory/heap.h>
#endif /* __AULAVIK_LIBK */

/**
 * Allocates size bytes of memory and returns a pointer to the start of the
 * allocated region. The memory is not initialized. If allocation failed for
 * any reason, NULL will be returned instead of a pointer. The pointer should
 * be passed to the free() function once it is no longer needed.
 *
 * @param size The amount of bytes to allocate
 * @return A pointer to the start of the allocated block, or NULL
 */
void* __ensure_result_use malloc(size_t size)
{
#ifdef __AULAVIK_LIBK
	return heap_alloc(size);
#else
	return NULL; // TODO
#endif /* __AULAVIK_LIBK */
}
