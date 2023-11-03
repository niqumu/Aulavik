#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
 * Function: causes abnormal program termination to occur, unless the signal
 *      SIGABRT is being caught and the signal handler does not return
 *
 * Return: It is impossible for abort() to return to its caller
 ****************************************************************************/
__attribute__((__noreturn__))
void abort(void) {
#ifdef __is_libk
        // TODO add proper kernel panic
        printf("kernel: panic: abort()\n");
        asm volatile("hlt");
#else
        // TODO abnormally terminate the process similar to SIGABRT
        printf("abort()\n");
#endif
       while (1) { }
      __builtin_unreachable();
} 
