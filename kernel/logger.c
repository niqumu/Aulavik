/*====------------- logger.c - Kernel logger implementation --------------====*\
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

#include <kernel/logger.h>

#include <stdarg.h> /* va_list, va_start(), va_end() */
#include <stdio.h> /* printf() */
#include <stdlib.h> /* malloc(), free() */

#include <kernel/driver/serial.h> /* serial_write_string() */

void k_debug(const char* restrict format, ...)
{
	printf(PREFIX_DEBUG);

	va_list args;
	va_start(args, format);

#ifdef SERIAL_LOGGING_ENABLED

#ifdef SERIAL_LOGGING_ANSI
	serial_write_string(SERIAL_LOGGING_PORT, PREFIX_DEBUG);
#else
	serial_write_string(SERIAL_LOGGING_PORT, PREFIX_DEBUG_M);
#endif /* SERIAL_LOGGING_ANSI */

	char *str = malloc(256);
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
	free(str);
#endif /* SERIAL_LOGGING_ENABLED */

	vprintf(format, args);
	va_end(args);
	printf("\n");
}

void k_print(const char* __restrict format, ...)
{
	va_list args;
	va_start(args, format);

#ifdef SERIAL_LOGGING_ENABLED
	char *str = malloc(256);
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
	free(str);
#endif /* SERIAL_LOGGING_ENABLED */

	vprintf(format, args);
	va_end(args);
	printf("\n");
}

void k_ok(const char* restrict format, ...)
{
	printf(PREFIX_OK);

	va_list args;
	va_start(args, format);

#ifdef SERIAL_LOGGING_ENABLED

#ifdef SERIAL_LOGGING_ANSI
	serial_write_string(SERIAL_LOGGING_PORT, PREFIX_OK);
#else
	serial_write_string(SERIAL_LOGGING_PORT, PREFIX_OK_M);
#endif /* SERIAL_LOGGING_ANSI */

	char *str = malloc(256);
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
	free(str);
#endif /* SERIAL_LOGGING_ENABLED */

	vprintf(format, args);
	va_end(args);

	printf("\n");
}

void k_warn(const char* restrict format, ...)
{
	printf(PREFIX_WARN);

	va_list args;
	va_start(args, format);

#ifdef SERIAL_LOGGING_ENABLED

#ifdef SERIAL_LOGGING_ANSI
	serial_write_string(SERIAL_LOGGING_PORT, PREFIX_WARN);
#else
	serial_write_string(SERIAL_LOGGING_PORT, PREFIX_WARN_M);
#endif /* SERIAL_LOGGING_ANSI */

	char *str = malloc(256);
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
	free(str);
#endif /* SERIAL_LOGGING_ENABLED */

	vprintf(format, args);
	va_end(args);

	printf("\n");
}

void k_error(const char* restrict format, ...)
{
	printf(PREFIX_ERROR);

	va_list args;
	va_start(args, format);

#ifdef SERIAL_LOGGING_ENABLED

#ifdef SERIAL_LOGGING_ANSI
	serial_write_string(SERIAL_LOGGING_PORT, PREFIX_ERROR);
#else
	serial_write_string(SERIAL_LOGGING_PORT, PREFIX_ERROR_M);
#endif /* SERIAL_LOGGING_ANSI */

	char *str = malloc(256);
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
	free(str);
#endif /* SERIAL_LOGGING_ENABLED */

	vprintf(format, args);
	va_end(args);

	printf("\n");
}