/*====------------------ scheduler.c - Task scheduler --------------------====*\
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

#include <kernel/task/scheduler.h>

#include <stdlib.h>

#include <kernel/logger.h>
#include <string.h>
#include "kernel/loader/loader.h"

struct process *first_process;
struct process *current_process;

bool initialized = false;

uint16_t next_pid = 1;

bool scheduler_initialized(void)
{
	return initialized;
}

void scheduler_dump(void)
{
	struct process *process = first_process;

	while (true) {

		char *last_name = process->last_process ?
			process->last_process->name : "(none)";
		char *next_name = process->next_process ?
			process->next_process->name : "(none)";

		k_print("process: %s, pid %d", process->name, process->pid);
		k_print("  last: %s, next %s", last_name, next_name);

		if (!process->next_process)
			return;

		process = process->next_process;
	}
}

_Noreturn void scheduler_idle(void)
{
	k_debug("idle task started");
	while (true)
		asm("hlt");
}

_Noreturn void scheduler_test(void)
{
	k_debug("test task started");
	while (true)
		asm("hlt");
}

extern void scheduler_load_state(struct cpu_state state);

void scheduler_switch_next(struct cpu_state old_state)
{
	/* if there is only one process, do nothing */
	if (current_process == first_process && !first_process->next_process)
		return;

	current_process->state = old_state;
	current_process->status = READY;

	if (current_process->next_process) {
		current_process = current_process->next_process;
	} else {
		current_process = first_process;
	}

	k_debug("scheduler: switch to pid %d (%s)",
		current_process->pid, current_process->name);

	current_process->status = ACTIVE;
	scheduler_load_state(current_process->state);
}

/**
 * Terminates the current process, freeing it and removing it from the task
 * queue, and idles until the next task switch
 * @param exit_code The exit code of the terminated process
 */
_Noreturn void scheduler_exit(int exit_code)
{
	asm("cli");

	current_process->status = FINISHED;
	current_process->parent->status = READY;

	k_ok("scheduler: Process %d (%s) finished with code %d",
	     current_process->pid, current_process->name, exit_code);

	if (current_process->last_process != NULL) {
		current_process->last_process->next_process =
			current_process->next_process;
	}

	if (current_process->next_process != NULL) {
		current_process->next_process->last_process =
			current_process->last_process;
	}

	if (current_process == first_process) {
		first_process = current_process->next_process;
	}

	free((void *) current_process->state.ebp);
	free(current_process);

	asm("sti");
	while (true)
		asm("hlt");
}

/**
 * Creates and switches to a new process
 * @param name The name of the new process
 * @param entry A pointer to the new process's entry point, of the signature
 *      void (int, char**)
 * @param argc The number of command line arguments to pass to the new process
 * @param argv The command line arguments to pass to the new process
 */
void scheduler_spawn(char *name, void (*entry) (int, char **),
		     int argc, char **argv)
{
	asm("cli");

	struct process *process = calloc(sizeof(struct process));
	process->status = READY;
	process->pid = next_pid++;
	memcpy(process->name, name, 63);

	if (current_process != NULL) {
		current_process->status = WAITING;
		process->parent = current_process;
	}

	if (first_process != NULL) {
		first_process->last_process = process;
		process->next_process = first_process;
	}

	process->state.ebp = (uint32_t) calloc(PROCESS_STACK_SIZE);
	process->state.esp = process->state.ebp;
	process->state.eflags = 0x200000 | 0x200 | 0x2;
	process->state.eip = (uint32_t) entry;

	k_debug("Allocated stack for %s at %x", process->name, process->state.ebp);

	current_process = process;
	first_process = process;

	k_debug("Loading state");
	scheduler_load_state(current_process->state);
}

void scheduler_create_idle()
{
	struct process *idle = calloc(sizeof(struct process));
	idle->status = READY;
	idle->pid = next_pid++;
	memcpy(idle->name, "idle", 63);

	idle->state.ebp = (uint32_t) calloc(PROCESS_STACK_SIZE);
	idle->state.esp = idle->state.ebp;
	idle->state.eflags = 0x200000 | 0x200 | 0x2;
	idle->state.eip = (uint32_t) &scheduler_idle;

	first_process = idle;
	current_process = idle;
}

void scheduler_create_test()
{
	struct process *idle = calloc(sizeof(struct process));
	idle->status = READY;
	idle->pid = next_pid++;
	memcpy(idle->name, "test", 63);

	idle->state.ebp = (uint32_t) calloc(PROCESS_STACK_SIZE);
	idle->state.esp = idle->state.ebp;
	idle->state.eflags = 0x200000 | 0x200 | 0x2;
	idle->state.eip = (uint32_t) &scheduler_test;

	first_process->next_process = idle;
	idle->last_process = first_process;
}

void scheduler_init(void)
{
	scheduler_create_idle();
	scheduler_create_test();
	k_ok("Started scheduler");
	initialized = true;
}
