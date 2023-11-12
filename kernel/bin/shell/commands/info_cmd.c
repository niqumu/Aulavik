/*====------------------- info_cmd.c - Information tool ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include "info_cmd.h"

#include <cpuid.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <kernel/driver/ata.h>
#include <kernel/driver/fat32.h>
#include <kernel/driver/pci.h>
#include <kernel/bin/shell.h>

struct shell_command info_command;

void cpuid(void)
{
	unsigned char chars[16];
	uint32_t eax, ebx, ecx, edx;
	asm volatile ("mov $0, %eax");
	asm volatile ("cpuid");
	asm volatile ("mov %%eax, %0" : "=a" (eax));
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


	unsigned int regs[12];
	char model[sizeof(regs) + 1];

	__cpuid(0x80000000, regs[0], regs[1], regs[2], regs[3]);

	if (regs[0] < 0x80000004)
		return;

	__cpuid(0x80000002, regs[0], regs[1], regs[2], regs[3]);
	__cpuid(0x80000003, regs[4], regs[5], regs[6], regs[7]);
	__cpuid(0x80000004, regs[8], regs[9], regs[10], regs[11]);

	memcpy(model, regs, sizeof(regs));
	model[sizeof(regs)] = '\0';

	printf("Vendor: %s\n", chars);
	printf("  Model name: %s\n", model);

	printf("    Max leaf: %x\n", eax);

	asm volatile ("mov $1, %eax");
	asm volatile ("cpuid");
	asm volatile ("mov %%eax, %0" : "=a" (eax));

	printf("    Stepping version: %d\n", eax & 0b1111);
	printf("    Model ID: %d\n", (eax > 4) & 0b1111);
	printf("    Family ID: %d\n", (eax > 8) & 0b1111);
	printf("    Type: %d\n", (eax > 12) & 0b11);
}

bool execute_info_command(char *cmd)
{
	if (!cmd[4]) {
		printf("Invalid usage: supply an option\n");
		printf("Valid options: \"ata\", \"fat\", "
		       "\"cpu\", \"pci\"\n");
		return false;
	}

	switch (cmd[5]) {
	case 'a':
		ata_dump_info();
		return true;
	case 'c':
		cpuid();
		return true;
	case 'f':
		fat32_dump_info();
		return true;
	case 'p':
		pci_list_devices();
		return true;
	default:
		printf("Unrecognized option: \"%s\"\n", &cmd[5]);
		printf("Valid options: \"ata\", \"fat\", "
		       "\"cpu\", \"pci\"\n");
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