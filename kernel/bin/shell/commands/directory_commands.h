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

#ifndef SHELL_DIRECTORY_COMMANDS_H
#define SHELL_DIRECTORY_COMMANDS_H

struct shell_command* cat_init(void);

struct shell_command* cd_init(void);

struct shell_command* ls_init(void);

#endif /* SHELL_DIRECTORY_COMMANDS_H */
