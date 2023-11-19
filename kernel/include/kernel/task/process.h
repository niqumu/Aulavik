/*====----------- process.h - Process structs and definitions ------------====*\
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

#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <stdint.h>

#define PROCESS_STACK_SIZE      1048576         /* 1 MiB */

struct cpu_state_in {
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
} __attribute((packed));

struct cpu_state_out {
//	uint32_t eflags;
//	uint32_t cs;
//	uint32_t eip;
//	uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
} __attribute((packed));

enum process_status {
#define PROCESS_STATUS_ACTIVE   0
	ACTIVE = 0,     /* the process is currently using cpu cycles */

#define PROCESS_STATUS_READY    1
	READY = 1,      /* the process is ready, awaiting cpu cycles */

#define PROCESS_STATUS_WAITING  2
	WAITING = 2,    /* the process is waiting for something else */

#define PROCESS_STATUS_FINISHED 3
	FINISHED = 3    /* the process has finished execution */
};

struct process {
	struct cpu_state_in state;

	char name[64];
	uint16_t pid;
	enum process_status status;

	struct process *parent;

	struct process *last_process;
	struct process *next_process;
};

#endif /* KERNEL_PROCESS_H */
