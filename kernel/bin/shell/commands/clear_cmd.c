/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include "clear_cmd.h"

#include <stdbool.h>

#include <kernel/graphics/graphics.h>
#include <kernel/kernel.h>
#include <kernel/terminal.h>

struct shell_command clear_command;

bool execute_clear_command(char *cmd)
{
	terminal_clear();
	return true;
}

struct shell_command* clear_init(void)
{
	clear_command.name = "clear";
	clear_command.command = &execute_clear_command;
	return &clear_command;
}