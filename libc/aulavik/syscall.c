/*====------------ syscall.c - Aulavik syscall implementation ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <aulavik/syscall.h>
#include <stdio.h>

ssize_t syscall(void)
{
	unsigned long eax;
	ASM("int $128");
	ASM("mov %%eax, %0" : "=a" (eax));
	return eax;
}

/*
 * writes up to count bytes from the buffer, starting at buf, to the file
 * referred to by the file descriptor fd.
 */
ssize_t syscall_write(unsigned int fd, const void *buf, size_t count)
{
//	printf("2: \"%s\"\n", buf);

	ASM("mov %0, %%eax" :: "a" (SYSCALL_WRITE));
	ASM("mov %0, %%edi" :: "g" (fd));
	ASM("mov %0, %%esi" :: "g" (buf));
	ASM("mov %0, %%edx" :: "g" (count));

	/* return -1 if we got 1 when invoking the syscall */
	if (syscall() == SYSCALL_KERNEL_ERR)
		return SYSCALL_ERR;

	/* return the amount of bytes written */
	return count;
}