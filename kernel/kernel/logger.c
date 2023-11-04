/*====------------- logger.c - Kernel logger implementation --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/logger.h>

#include <stdarg.h> /* va_list, va_start(), va_end() */
#include <stdio.h> /* printf() */
#include <string.h> /* strcat() */

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
#endif
	char *str;
	memset(str, 0, 64); // todo this needs to be malloc()!
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
#endif

	vprintf(format, args);
	va_end(args);
	printf("\n");
}

void k_print(const char* __restrict format, ...)
{
	va_list args;
	va_start(args, format);

#ifdef SERIAL_LOGGING_ENABLED
	char *str;
	memset(str, 0, 64); // todo this needs to be malloc()!
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
#endif

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
#endif
	char *str;
	memset(str, 0, 64); // todo this needs to be malloc()!
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
#endif

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
#endif
	char *str;
	memset(str, 0, 64); // todo this needs to be malloc()!
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
#endif

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
#endif
	char *str;
	memset(str, 0, 64); // todo this needs to be malloc()!
	vsprintf(str, format, args);

	serial_write_string(SERIAL_LOGGING_PORT, str);
	serial_write_string(SERIAL_LOGGING_PORT, "\n");
#endif

	vprintf(format, args);
	va_end(args);

	printf("\n");
}