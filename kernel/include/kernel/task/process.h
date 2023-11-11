/*====----------- process.h - Process structs and definitions ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef KERNEL_PROCESS_H
#define KERNEL_PROCESS_H

#include <stdint.h>

struct cpu_state {
	uint32_t eax, ebx, ecx, edx, esi;
	uint32_t edi, esp, esb, ebp, eip;
	uint32_t eflags;
};

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
	struct cpu_state state;

	char *name;
	uint16_t pid;
	enum process_status status;

	struct process *next_process;
};

#endif /* KERNEL_PROCESS_H */
