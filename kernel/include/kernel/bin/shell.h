/*====------------------- shell.h - Basic shell header -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef BIN_SHELL_H
#define BIN_SHELL_H

#include <stdbool.h>

typedef bool (*execute_command)(char *cmd);

struct shell_command {
	char *name;
	execute_command command;
};

extern struct shell_command commands[16];

void shell_main(void);

#endif /* BIN_SHELL_H */