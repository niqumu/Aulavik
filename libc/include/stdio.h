/*====------------- stdio.h - Standard I/O functions header --------------====*\
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

#ifndef _STDIO_H
#define _STDIO_H        1

#include <sys/cdefs.h>

#include <stdarg.h> /* va_list */
#include <stdint.h> /* uint64_t */

typedef uint64_t FILE;

#define EOF (-1)

/*
 * Define macros for standard streams of type FILE *, associated with the
 * respective stream
 * @ ISO/IEC 9899:201 §7.21.1.3
 */
#define stdout  ((FILE *) 0)
#define stdin   ((FILE *) 1)
#define stderr  ((FILE *) 2)

__BEGIN_DECLS

/**
 * Writes the formatted output of the format string pointed to by str and the
 * provided arguments into stdout
 *
 * @param str A multibyte character sequence, optionally with codes to indicate
 *      the formatting and insertion of an additional provided argument
 * @return The number of characters written, or, if an error occurs, EOF
 */
int printf(const char* __restrict str, ...);

/**
 * Writes the provided character to stdout.
 * @param i The character to write
 * @return The character written, or, if a write error occurs, EOF.
 */
int putchar(int i);

/**
 * Writes the string pointed to by str to stdout, and appends a new-line.
 * @param str A pointer to the string to write
 * @return Zero, or EOF if a write error occured
 */
int puts(const char *str);

/**
 * Writes the formatted output of the format string pointed to by str and the
 * provided arguments into the string pointed to by dest.
 *
 * @param dest A pointer to a sufficiently large string to write into
 * @param str A multibyte character sequence, optionally with codes to indicate
 *      the formatting and insertion of an additional provided argument
 * @return The number of characters written, or, if an error occurs, EOF
 */
int sprintf(char* __restrict dest, const char* __restrict str, ...);

/**
 * Writes the formatted output of the format string pointed to by str and the
 * provided arguments, parameters, into stdout
 *
 * @param str A multibyte character sequence, optionally with codes to indicate
 *      the formatting and insertion of an additional provided argument
 * @param parameters A variable argument list of values to insert/format
 * @return The number of characters written, or, if an error occurs, EOF
 */
int vprintf(const char* __restrict str, va_list args);

/**
 * Writes the formatted output of the format string pointed to by str and the
 * provided arguments into the string pointed to by dest.
 *
 * @param dest A pointer to a sufficiently large string to write into
 * @param str A multibyte character sequence, optionally with codes to indicate
 *      the formatting and insertion of an additional provided argument
 * @param parameters A variable argument list of values to insert/format
 * @return The number of characters written, or, if an error occurs, EOF
 */
int vsprintf(char* __restrict dest, const char* __restrict str, va_list args);

__END_DECLS

#endif /* !_STDIO_H */
