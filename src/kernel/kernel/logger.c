/*====------------- logger.c - Kernel logger implementation --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdarg.h> /* va_list, va_start(), va_end() */
#include <stdio.h> /* printf() */

#include <kernel/logger.h>

#define FG_GREEN        "\e[92m"
#define FG_ORANGE       "\e[93m"
#define FG_RED          "\e[91m"
#define FG_DARK_GRAY    "\e[90m"
#define FG_GRAY         "\e[37m"

void k_debug(const char* restrict format, ...)
{
	printf("[ " FG_DARK_GRAY "DBG" FG_GRAY " ] ");

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	printf("\n");
}

void k_ok(const char* restrict format, ...)
{
	printf("[ " FG_GREEN "OK" FG_GRAY " ] ");

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	printf("\n");
}

void k_warn(const char* restrict format, ...)
{
	printf("[ " FG_ORANGE "WARN" FG_GRAY " ] ");

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	printf("\n");
}

void k_error(const char* restrict format, ...)
{
	printf("[ " FG_RED "ERR" FG_GRAY " ] ");

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	printf("\n");
}