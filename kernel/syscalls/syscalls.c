/*====------------- syscalls.c - First-layer syscall handler -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/syscalls/syscalls.h>

#include <stddef.h>

#include <aulavik/syscall.h>

#include <kernel/terminal.h>
#include <stdio.h>
#include "kernel/logger.h"

void syscalls_write(uint32_t edi, uint32_t esi, uint32_t edx)
{
//	printf("4: \"%s\"\n", (char*) esi);

	char *buffer = (char *) esi;
	for (size_t i = 0; i < edx; i++) {
		terminal_putc(buffer[i]);
	}

	// mark the syscall as successful
	asm volatile ("mov $0, %eax");
}

void syscalls_handle(uint32_t eax, uint32_t edi, uint32_t esi, uint32_t edx)
{
//	printf("3: \"%s\"\n", (char*) esi);

	switch (eax) {
	case SYSCALL_WRITE:
		syscalls_write(edi, esi, edx);
		break;
	}
}