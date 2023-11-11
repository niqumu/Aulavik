/*====------------- abort.c - stdlib.h abort implementation --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h>
#include <stdlib.h>

#ifdef __AULAVIK_LIBK
  #include <kernel/kernel.h>
#endif /* __AULAVIK_LIBK */

__attribute__((__noreturn__))
void abort(void) {
#ifdef __AULAVIK_LIBK
	panic("Abort");
#else
        // TODO abnormally terminate the process similar to SIGABRT
        printf("abort()\n");
#endif /* __AULAVIK_LIBK */

	while (1)
		;

      __builtin_unreachable();
} 
