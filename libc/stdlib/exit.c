/*====-------------- exit.c - stdlib.h exit implementation ---------------====*\
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

#include <aulavik/syscall.h>

_Noreturn void exit(int status)
{
	syscall_exit(status);

	__builtin_unreachable();
}
