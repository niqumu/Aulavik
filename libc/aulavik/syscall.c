/*====------------ syscall.c - Aulavik syscall implementation ------------====*\
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
ssize_t syscall_write(const FILE *file, const void *buf, size_t count)
{
	ASM("mov %0, %%eax" :: "a" (SYSCALL_WRITE));
	ASM("mov %0, %%edi" :: "g" (&file));
	ASM("mov %0, %%esi" :: "g" (buf));
	ASM("mov %0, %%edx" :: "g" (count));

	/* return -1 if we got 1 when invoking the syscall */
	if (syscall() == SYSCALL_KERNEL_ERR)
		return SYSCALL_ERR;

	/* return the amount of bytes written */
	return count;
}

_Noreturn void syscall_exit(int status)
{
	ASM("mov %0, %%eax" :: "a" (SYSCALL_EXIT));
	ASM("mov %0, %%edi" :: "g" (status));
	syscall();

	__builtin_unreachable();
}