/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
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

#include "directory_commands.h"
#include "../../../../libc/include/stdio.h"
#include "../../../../libc/include/stdlib.h"
#include "../../../../libc/include/string.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/bin/shell.h>
#include <kernel/driver/fat32.h>

struct shell_command cat_command;
struct shell_command cd_command;
struct shell_command ls_command;

bool execute_ls_command(char *cmd)
{
	printf("Directory \"%s\"\n\n", get_current_directory()->name);

	char *time_buffer = malloc(32);
	char *date_buffer = malloc(32);

	for (uint32_t i = 0; i < get_current_directory()->entry_count; i++) {

		struct fat32_directory_entry entry =
			get_current_directory()->entries[i];

		memset(time_buffer, 0, 32);
		memset(date_buffer, 0, 32);

		/* print modified date and time */
		printf("%s  ", fat32_date_dump(date_buffer, entry.modify_date));
		printf("%s  ", fat32_time_dump(time_buffer, entry.modify_time));

		if (entry.attributes & FAT_ATTRIBUTE_DIRECTORY) {
			printf(" (Dir)   ");
		} else {
			printf("         ");
		}

		printf("%s\n", entry.display_name);
	}

	printf("\n%d entries\n", get_current_directory()->entry_count);

	return true;
}

bool execute_cd_command(char *cmd)
{
	if (strlen(cmd) <= 3) {
		printf("\e[91mNo directory provided\e[0m\n");
		return false;
	}

	size_t cmd_len = strlen(cmd);
	char *arg = malloc(cmd_len);
	memcpy(arg, &cmd[3], cmd_len - 3);

	/* make / go to the root */
	if (strcmp(arg, "/") == 0) {
		struct fat32_directory *cd = get_current_directory();
		fat32_read_directory(fat32_get_drive()->root_cluster,
		                     cd->entries, &cd->entry_count);
		cd->name[0] = '/';
		cd->name[1] = '\0';
//		free(arg);
		return true;
	}

	for (uint32_t i = 0; i < get_current_directory()->entry_count; i++) {

		struct fat32_directory_entry entry =
			get_current_directory()->entries[i];

		if (!(entry.attributes & FAT_ATTRIBUTE_DIRECTORY))
			continue; /* we only care about directories */

		if (strcmp(entry.display_name, arg) == 0) {
			struct fat32_directory *cd = get_current_directory();
			fat32_read_directory(entry.first_cluster,
					     cd->entries, &cd->entry_count);
			memmove(cd->name, entry.display_name, strlen(entry.display_name));
			cd->name[strlen(entry.display_name)] = '\0';
//			free(arg);
			return true;
		}
	}

	printf("\e[91mNo such directory \"%s\"\e[0m\n", arg);
//	free(arg);
	return false;
}

bool execute_cat_command(char *cmd)
{
	if (strlen(cmd) <= 4) {
		printf("\e[91mNo file provided\e[0m\n");
		return false;
	}

	size_t cmd_len = strlen(cmd);
	char *arg = malloc(cmd_len);
	memcpy(arg, &cmd[4], cmd_len - 4);

	for (uint32_t i = 0; i < get_current_directory()->entry_count; i++) {

		struct fat32_directory_entry entry =
			get_current_directory()->entries[i];

		if (entry.attributes & FAT_ATTRIBUTE_DIRECTORY)
			continue; /* we only care about files */

		if (strcmp(entry.display_name, arg) == 0) {
			uint8_t buffer[1024];
			uint32_t *bytes;
			fat32_read_file(entry.first_cluster, buffer, bytes);

			printf("%s\n", buffer);

//			free(arg);
			return true;
		}
	}

	printf("\e[91mNo such file \"%s\"\e[0m\n", arg);
	return false;
//	free(arg);
}

struct shell_command* cat_init(void)
{
	cat_command.name = "cat";
	cat_command.command = &execute_cat_command;
	return &cat_command;
}

struct shell_command* cd_init(void)
{
	cd_command.name = "cd";
	cd_command.command = &execute_cd_command;
	return &cd_command;
}

struct shell_command* ls_init(void)
{
	ls_command.name = "ls";
	ls_command.command = &execute_ls_command;
	return &ls_command;
}