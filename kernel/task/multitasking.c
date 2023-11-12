/*====----------------- multitasking.c - Task scheduler ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/task/multitasking.h>

#include <kernel/logger.h>

struct process processes[127];
struct process *current_process;
uint16_t process_count = 0;

struct cpu_state multitasking_switch_next(struct cpu_state old_state)
{
	current_process->state = old_state;
	current_process->status = READY;

	if (current_process->next_process) {
		current_process = current_process->next_process;
	} else {
		current_process = &processes[0];
	}

	current_process->status = ACTIVE;
	return current_process->state;
}

void multitasking_init(void)
{
	processes[0].status = ACTIVE;
	processes[0].name = "Aulavik";
	processes[0].pid = 0;
	process_count++;
	current_process = &processes[0];

	k_ok("Started scheduler");
}