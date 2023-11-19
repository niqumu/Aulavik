/*====---------------- scheduler.h - Task scheduler header ---------------====*\
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

#ifndef KERNEL_SCHEDULER_H
#define KERNEL_SCHEDULER_H

#include <stdbool.h>

#include <kernel/task/process.h>

bool scheduler_initialized(void);

void scheduler_switch_next(struct cpu_state old_state);

void scheduler_exit(int exit_code);

/**
 * Creates and switches to a new process
 * @param name The name of the new process
 * @param entry A pointer to the new process's entry point, of the signature
 *      void (int, char**)
 * @param argc The number of command line arguments to pass to the new process
 * @param argv The command line arguments to pass to the new process
 */
void scheduler_spawn(char *name, void (*entry) (int, char **),
		     int argc, char *argv[]);

void scheduler_init(void);

#endif /* KERNEL_SCHEDULER_H */
