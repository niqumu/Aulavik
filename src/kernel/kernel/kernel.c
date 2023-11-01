#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void)
{
        terminal_initialize();
        printf("Hello, meaty kernel!\n\n");

        int a = 15, b = 273;
        printf("printf tests:\n");

        // Triggers the bug
        printf("Hard: %d, compiled: %d, dynamic: %d!\n", 5, 5 - 11, a + b);
        
        //printf("a: %c, b: %c, c: %c, d:\n", 'a', 'b', 'c');
        printf("0x100: %x, 0x0a9: %x\n", 0x100, 0xa9);
        printf("Just a normal string...\n");
}
