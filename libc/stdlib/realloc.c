/*====----------- realloc.c - stdlib.h realloc implementation ------------====*\
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

#include <string.h>

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
void* realloc(void *ptr, size_t size)
{
	void *p = malloc(size);

	if (ptr != NULL && p != NULL) {
		memcpy(p, ptr, size);
		free(ptr);
	}

	return p;
}
