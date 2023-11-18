/*====-------------- printf - stdio.h printf implementation --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h>
#include <stdarg.h> /* va_list */

int printf(const char* restrict format, ...)
{
	va_list args;
	va_start(args, format);
	int i = vprintf(format, args);
	va_end(args);
	return i;
}