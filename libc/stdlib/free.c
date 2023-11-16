/*====-------------- free.c - stdlib.h free implementation ---------------====*\
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
 * Marks the memory region pointed to by ptr as available for further
 * allocation. This must be done every time memory requsted via malloc() or
 * calloc() is no longer needed. Accessing memory after it has been free()'d is
 * undefined.
 * @param ptr A pointer to the memory to mark as free
 */
void free(void *ptr)
{
#ifdef __AULAVIK_LIBK
	return heap_free(ptr);
#else
	// TODO
#endif /* __AULAVIK_LIBK */
}