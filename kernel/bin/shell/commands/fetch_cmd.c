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

#define COLOR_LOGO      "\e[91m"
#define COLOR_PREFIX    "\e[94m"
#define COLOR_TEXT      "\e[97m"
#define COLOR_RESET     "\e[0m"

struct shell_command fetch_command;

bool execute_fetch_command(char *cmd)
{
	printf(COLOR_LOGO "      ____________\n"
	       COLOR_LOGO "     /\\  ________ \\\n"
	       COLOR_LOGO "    /  \\ \\______/\\ \\\n"
	       COLOR_LOGO "   / /\\ \\ \\  / /\\ \\ \\        " COLOR_PREFIX "OS: " COLOR_TEXT "%s\n"
	       COLOR_LOGO "  / / /\\ \\ \\/ / /\\ \\ \\       " COLOR_PREFIX "Bootloader: " COLOR_TEXT "%s\n"
	       COLOR_LOGO " / / /__\\_\\/ / /__\\_\\ \\      " COLOR_PREFIX "Resolution: " COLOR_TEXT "%dx%d\n"
	       COLOR_LOGO "/ /_/_______/ /________\\     " COLOR_PREFIX "Memory: " COLOR_TEXT "%d MiB\n"
	       COLOR_LOGO "\\ \\ \\______ \\ \\______  /\n"
	       COLOR_LOGO " \\ \\ \\  / /\\ \\ \\  / / /\n"
	       COLOR_LOGO "  \\ \\ \\/ / /\\ \\ \\/ / /       \e[;40m   \e[;41m   \e[;42m   "
			  "\e[;43m   \e[;44m   \e[;45m   \e[;46m   \e[;47m   " COLOR_RESET "\n"
	       COLOR_LOGO "   \\ \\/ / /__\\_\\/ / /        \e[;100m   \e[;101m   \e[;102m   "
			  "\e[;103m   \e[;104m   \e[;105m   \e[;106m   \e[;107m   " COLOR_RESET "\n"
	       COLOR_LOGO "    \\  / /______\\/ /\n"
	       COLOR_LOGO "     \\/___________/\n", "Aulavik", mb_info->boot_loader_name,
	       graphics_get_global_rctx()->width, graphics_get_global_rctx()->height,
	       mb_info->mem_upper / 1024);

//	printf("\n");
//
//	printf(COLOR_PREFIX "OS: " COLOR_TEXT "Aulavik\n");
//	printf(COLOR_PREFIX "Bootloader: " COLOR_TEXT "%s\n",
//	       mb_info->boot_loader_name);
//	printf(COLOR_PREFIX "Resolution: " COLOR_TEXT "%dx%d\n",
//	       graphics_get_context()->width, graphics_get_context()->height);
//	printf(COLOR_PREFIX "Memory: " COLOR_TEXT "%d MiB\n\n",
//	       mb_info->mem_upper / 1024);
//
//	/* colors */
//	for (int i = 40; i < 48; i++) {
//		printf("\e[;%dm   ", i);
//	}
//	printf("\n");
//	for (int i = 100; i < 108; i++) {
//		printf("\e[;%dm   ", i);
//	}

	printf("\n" COLOR_RESET);
	return true;
}

struct shell_command* fetch_init(void)
{
	fetch_command.name = "fetch";
	fetch_command.command = &execute_fetch_command;
	return &fetch_command;
}