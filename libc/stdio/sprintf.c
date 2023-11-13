/*====------------ sprintf.c - stdio.h sprintf implementation ------------====*\
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

#include <stdio.h>
#include <stdlib.h>

int sprintf(char* __restrict dest, const char* __restrict str, ...)
{
	va_list args;
	va_start(args, str);
	int ret = vsprintf(dest, str, args);
	va_end(args);
	return ret;
}