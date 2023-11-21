/*====------------ putchar.c - stdio.h putchar implementation ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h>

#ifdef __AULAVIK_LIBK
  #include <kernel/terminal.h>
#else
  #include <aulavik/syscall.h>
#endif /* __AULAVIK_LIBK */

int putchar(int i)
{
#ifdef __AULAVIK_LIBK
        terminal_putc((char) i);
#else
	char c = (char) i;
	syscall_write(stdout, &c, 1);
#endif /* __AULAVIK_LIBK */
        return i;
}
