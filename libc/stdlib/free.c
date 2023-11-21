/*====-------------- free.c - stdlib.h free implementation ---------------====*\
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
