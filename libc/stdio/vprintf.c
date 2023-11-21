/*====------------- vprintf - stdio.h vprintf implementation -------------====*\
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

#include <stdlib.h> /* malloc() */
#include <string.h> /* strlen() */

#ifdef __AULAVIK_LIBK
	#include <kernel/terminal.h>
#else
	#include <aulavik/syscall.h>
#endif /* __AULAVIK_LIBK */

/**
 * Writes the formatted output of the format string pointed to by str and the
 * provided arguments, parameters, into stdout
 *
 * @param str A multibyte character sequence, optionally with codes to indicate
 *      the formatting and insertion of an additional provided argument
 * @param parameters A variable argument list of values to insert/format
 * @return The number of characters written, or, if an error occurs, EOF
 */
int vprintf(const char* restrict str, va_list parameters)
{
	void *addr = calloc(256); // TODO how big should this be?
	char *dest = (char *) addr;
	int ret = vsprintf(dest, str, parameters);

#ifdef __AULAVIK_LIBK
	terminal_puts(dest);
#else
	syscall_write(stdout, dest, strlen(dest));
#endif /* __AULAVIK_LIBK */

	free(addr);
	return ret;
}
