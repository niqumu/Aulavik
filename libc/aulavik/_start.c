/*====-------------- _start.c - Aulavik program entry point --------------====*\
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

#include <aulavik.h>

#include <stdlib.h>

_Noreturn void _start(int argc, char *argv[])
{
	int status = aulavik_main(argc, argv);
	exit(status);

	__builtin_unreachable();
}
