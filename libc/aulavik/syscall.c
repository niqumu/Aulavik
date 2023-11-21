/*====------------ syscall.c - Aulavik syscall implementation ------------====*\
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
 * referred to by the file descriptor file.
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