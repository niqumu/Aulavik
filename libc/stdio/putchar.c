#include <stdio.h>

#ifdef __is_libk
#include <kernel/terminal.h>
#endif

int putchar(int i)
{
#ifdef __is_libk
        char c = (char) i;
        terminal_putc(c);
#else
        // TODO implement and write system calls
#endif
        return i;
}
