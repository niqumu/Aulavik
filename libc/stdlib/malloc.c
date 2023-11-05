/*====------------ malloc.c - stdlib.h malloc implementation -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdlib.h>

#include <kernel/memory/memory_manager.h>

void* malloc(size_t size)
{
	return memman_allocate(size);
}