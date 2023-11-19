/*====------------- abort.c - stdlib.h abort implementation --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that the relevant copyright
 * notice and permission notice shall be included in all copies or substantial
 * portions of this software and all documentation files.
 *
 * Refer to LICENSE for more information. These works are provided with
 * absolutely no warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdlib.h>

#include <stdio.h>

#ifdef __AULAVIK_LIBK
  #include <kernel/kernel.h>
#endif /* __AULAVIK_LIBK */

_Noreturn void abort(void) {
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
