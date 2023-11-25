/*====------------- fclose.c - stdio.h fclose implementation -------------====*\
 *
 * This file is a part of the Aulavik project. The Aulavik project is free 
 * software, licenced under the MIT License.
 *
 * Usage of these works (including, yet not limited to, reuse, modification,
 * copying, distribution, and selling) is permitted, provided that the relevant
 * copyright notice and permission notice (as specified in LICENSE) shall be
 * included in all copies or substantial portions of this software and all
 * documentation files.
 *
 * These works are provided "AS IS" with absolutely no warranty of any kind,
 * either expressed or implied.
 *
 * You should have received a copy of the MIT License alongside this software;
 * refer to LICENSE for information. If not, refer to https://mit-license.org.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h>

#ifdef __AULAVIK_LIBK
	#include <kernel/file/vfs.h>
#else
	#include <aulavik/syscall.h>
#endif /* __AULAVIK_LIBK */

int fclose(FILE *stream)
{
#ifdef __AULAVIK_LIBK
	return vfs_close((int) stream);
#else
	return syscall_close(stream);
#endif /* __AULAVIK_LIBK */
}
