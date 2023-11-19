/*====------------- syscalls.c - First-layer syscall handler -------------====*\
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

#include <kernel/syscalls/syscalls.h>

#include <aulavik/syscall.h>

#include <kernel/terminal.h>
#include <kernel/task/scheduler.h>

void syscalls_write(uint32_t edi, uint32_t esi, uint32_t edx)
{
	char *buffer = (char *) esi;
	for (size_t i = 0; i < edx; i++) {
		terminal_putc(buffer[i]);
	}

	// mark the syscall as successful
	asm volatile ("mov $0, %eax");
}

void syscalls_exit(int status)
{
	scheduler_exit(status);
}

void syscalls_handle(uint32_t eax, uint32_t edi, uint32_t esi, uint32_t edx)
{
	switch (eax) {
	case SYSCALL_WRITE:
		syscalls_write(edi, esi, edx);
		break;
	case SYSCALL_EXIT:
		syscalls_exit((int) edi);
		break;
	default:
		return;
	}
}