/*====--------------- shell.h - Basic shell implementation ---------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/bin/shell.h>

#include <stdio.h>
#include <string.h>

#include <kernel/driver/fat32.h>
#include <kernel/terminal.h>
#include "commands/info_cmd.h"
#include "commands/fetch_cmd.h"
#include "commands/clear_cmd.h"
#include "kernel/logger.h"

struct shell_command commands[16];

struct fat32_directory *current_directory;

bool alive = true;

struct fat32_directory* get_current_directory(void)
{
	return current_directory;
}

void shell_process_command(char *cmd)
{
	for (int i = 0; i < 16; i++) {
		struct shell_command command = commands[i];
		bool match = true;

		for (int j = 0; cmd[j] || command.name[j]; j++) {
			if (cmd[j] == ' ')
				break;

			if (cmd[j] != command.name[j]) {
				match = false;
				break;
			}
		}

		if (match) {
			command.command(cmd);
			return;
		}
	}

	terminal_puts("Unrecognized command\n");
}

void shell_main(void)
{
	current_directory = fat32_get_root();

	commands[0] = *info_init();
	commands[1] = *fetch_init();
	commands[2] = *clear_init();

	while (alive) {
		printf("\n\e[92m%s\e[90m$ \e[97m", current_directory->name);

		char *input = terminal_get_input();
		shell_process_command(input);
	}
}