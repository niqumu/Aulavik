/*====------------- vprintf - stdio.h vprintf implementation -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h>

#include <string.h>

int vprintf(const char* restrict format, va_list parameters)
{
	char *str;
	memset(str, 0, 256); // todo this NEEDS to be malloc
	int ret = vsprintf(str, format, parameters);

	while (*str)
		putchar(*str++);

	return ret;
}