/*====------------- logger.c - Kernel logger implementation --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h> /* printf() */
#include <string.h> /* strcat() */

#include <kernel/logger.h>

// TODO formatting %d %x etc doesn't work!
void k_ok(const char* restrict format, ...)
{
//	char msg[128] = {0};
//	strcat(msg, "[OK] ");
//	strcat(msg, format);
	printf("[ \e[92mOK \e[37m] %s\n", format);
}

void k_warn(const char* restrict format, ...)
{
	printf("[ \e[93mWARN \e[37m] %s\n", format);
}

void k_error(const char* restrict format, ...)
{
	printf("[ \e[91mERROR \e[37m] %s\n", format);
}