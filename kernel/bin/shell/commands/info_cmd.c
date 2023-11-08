/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include "info_cmd.h"

#include <stdbool.h>
#include <stdio.h>

#include <kernel/driver/pci.h>
#include <kernel/bin/shell.h>

struct shell_command command;

bool execute_info_command(char *cmd)
{
	if (!cmd[4]) {
		printf("Invalid usage: supply an argument\n");
		return false;
	}

	switch (cmd[5]) {
		case 'p':
			pci_list_devices();
			return true;
		default:
			printf("Unrecognized argument: \"%s\"\n", &cmd[5]);
			return false;
	}

	return true;
}

struct shell_command* info_init(void)
{
	command.name = "info";
	command.command = &execute_info_command;
	return &command;
}