/*====------------ strcat.c - string.h strcat implementation -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <string.h>

char* strcat(char *dest, const char *src)
{
	char *ptr = dest + strlen(dest);

	while (*src)
		*ptr++ = *src++;

	*ptr = 0;
	return dest;
}