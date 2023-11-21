/*====------------ putchar.c - stdio.h putchar implementation ------------====*\
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

#ifdef __AULAVIK_LIBK
	#include <kernel/terminal.h>
#else
	#include <aulavik/syscall.h>
#endif /* __AULAVIK_LIBK */

/**
 * Writes the provided character to stdout.
 * @param i The character to write
 * @return The character written, or, if a write error occurs, EOF.
 */
int putchar(int i)
{
#ifdef __AULAVIK_LIBK
        terminal_putc((char) i);
#else
	char c = (char) i;
	ssize_t ret = syscall_write(stdout, &c, 1);

	if (ret == SYSCALL_ERR)
		return EOF;
#endif /* __AULAVIK_LIBK */
        return i;
}
