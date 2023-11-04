#include <stdio.h>

#ifdef __is_libk
#include <kernel/driver/tty.h>
#endif

int putchar(int i)
{
#ifdef __is_libk
        char c = (char) i;
        terminal_putchar(c);
#else
        // TODO implement and write system calls
#endif
        return i;
}
