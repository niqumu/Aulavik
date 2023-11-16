/*====------------ malloc.c - stdlib.h malloc implementation -------------====*\
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
void* malloc(size_t size)
{
#ifdef __AULAVIK_LIBK
	return heap_alloc(size);
#else
	return NULL; // TODO
#endif /* __AULAVIK_LIBK */
}
