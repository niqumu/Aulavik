#include <stdio.h>

#ifdef __AULAVIK_LIBK
#include <kernel/terminal.h>
#endif

int putchar(int i)
{
#ifdef __AULAVIK_LIBK
        terminal_putc((char) i);
#else
        // TODO implement and write system calls
#endif /* __AULAVIK_LIBK */
        return i;
}
