/*====-------------- printf - stdio.h printf implementation --------------====*\
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

#include <stdio.h>

#include <stdarg.h> /* va_list */

/**
 * Writes the formatted output of the format string pointed to by str and the
 * provided arguments into stdout
 *
 * @param str A multibyte character sequence, optionally with codes to indicate
 *      the formatting and insertion of an additional provided argument
 * @return The number of characters written, or, if an error occurs, EOF
 */
int printf(const char* restrict format, ...)
{
	va_list args;
	va_start(args, format);
	int i = vprintf(format, args);
	va_end(args);
	return i;
}
