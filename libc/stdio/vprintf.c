/*====------------- vprintf - stdio.h vprintf implementation -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h>

#include <string.h>

#ifdef __AULAVIK_LIBC
  #include <aulavik/syscall.h>
#endif /* __AULAVIK_LIBC */

int vprintf(const char* restrict format, va_list parameters)
{
	char *str;
	memset(str, 0, 256); // todo this NEEDS to be malloc
	int ret = vsprintf(str, format, parameters);

#ifdef __AULAVIK_LIBK
	while (*str)
		putchar(*str++);
#else
	syscall_write(0, str, strlen(str));
#endif /* __AULAVIK_LIBK */

	return ret;
}