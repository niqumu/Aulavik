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

#include <kernel/driver/ata.h>
#include <kernel/driver/pci.h>
#include <kernel/bin/shell.h>

struct shell_command info_command;

void cpuid(void)
{
	unsigned char chars[16];
	uint32_t ebx, ecx, edx;
	asm volatile ("mov $0, %eax");
	asm volatile ("cpuid");
	asm volatile ("mov %%ebx, %0" : "=b" (ebx));
	asm volatile ("mov %%ecx, %0" : "=c" (ecx));
	asm volatile ("mov %%edx, %0" : "=d" (edx));

	chars[0] = ebx & 0xff;
	chars[1] = (ebx >> 8) & 0xff;
	chars[2] = (ebx >> 16) & 0xff;
	chars[3] = (ebx >> 24) & 0xff;
	chars[4] = edx & 0xff;
	chars[5] = (edx >> 8) & 0xff;
	chars[6] = (edx >> 16) & 0xff;
	chars[7] = (edx >> 24) & 0xff;
	chars[8] = ecx & 0xff;
	chars[9] = (ecx >> 8) & 0xff;
	chars[10] = (ecx >> 16) & 0xff;
	chars[11] = (ecx >> 24) & 0xff;
	chars[12] = '\0';

	printf("Vendor: %s\n", chars);
}

bool execute_info_command(char *cmd)
{
	if (!cmd[4]) {
		printf("Invalid usage: supply an option\n");
		printf("Valid options: \"ata\", \"cpu\", \"pci\"\n");
		return false;
	}

	switch (cmd[5]) {
		case 'a':
			ata_dump_info();
			return true;
		case 'c':
			cpuid();
			return true;
		case 'p':
			pci_list_devices();
			return true;
		default:
			printf("Unrecognized option: \"%s\"\n", &cmd[5]);
			printf("Valid options: \"ata\", \"cpu\", \"pci\"\n");
			return false;
	}

	return true;
}

struct shell_command* info_init(void)
{
	info_command.name = "info";
	info_command.command = &execute_info_command;
	return &info_command;
}