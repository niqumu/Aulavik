#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void)
{
        terminal_initialize();

        printf("kernel loaded!");

        //for (int i = 0; i < 50; i++) {
        //    printf("Testing scrolling: Current line: %d!\n", i);
        //}
}
