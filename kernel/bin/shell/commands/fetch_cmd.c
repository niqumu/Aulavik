/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include "fetch_cmd.h"

#include <stdbool.h>
#include <stdio.h>

#include <kernel/graphics/graphics.h>
#include <kernel/kernel.h>

struct shell_command fetch_command;

bool execute_fetch_command(char *cmd)
{
	printf("\e[96m      ____________\n"
	       "\e[96m     /\\  ________ \\\n"
	       "\e[96m    /  \\ \\______/\\ \\\n"
	       "\e[96m   / /\\ \\ \\  / /\\ \\ \\\n"
	       "\e[96m  / / /\\ \\ \\/ / /\\ \\ \\       \e[94mOS: \e[97m%s\n"
	       "\e[96m / / /__\\_\\/ / /__\\_\\ \\      \e[94mBootloader: \e[97m%s\n"
	       "\e[96m/ /_/_______/ /________\\     \e[94mResolution: \e[97m%dx%d\n"
	       "\e[96m\\ \\ \\______ \\ \\______  /     \e[94mMemory: \e[97m%d MiB\n"
	       "\e[96m \\ \\ \\  / /\\ \\ \\  / / /\n"
	       "\e[96m  \\ \\ \\/ / /\\ \\ \\/ / /\n"
	       "\e[96m   \\ \\/ / /__\\_\\/ / /\n"
	       "\e[96m    \\  / /______\\/ /\n"
	       "\e[96m     \\/___________/\n", "Aulavik", mb_info->boot_loader_name,
	       graphics_get_context()->width, graphics_get_context()->height,
	       mb_info->mem_upper / 1024);
}

struct shell_command* fetch_init(void)
{
	fetch_command.name = "fetch";
	fetch_command.command = &execute_fetch_command;
	return &fetch_command;
}