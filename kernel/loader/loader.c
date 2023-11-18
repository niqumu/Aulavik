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

#include <kernel/loader/loader.h>

#include <stdlib.h>

#include <kernel/logger.h>
#include <kernel/loader/elf.h>

uint8_t *executable;

void loader_load(struct fat32_directory_entry file)
{
	/* read and load into memory */
	executable = malloc(file.size);
	fat32_read_file(file.first_cluster, executable, NULL);
	k_ok("Loaded executable to %x", executable);
	struct elf_file elf = elf_parse(executable, file.size);

	elf_dump(elf);

	if (elf.architecture != ELF_ARCH_X86) {
		k_error("Unsupported architecture %x!", elf.architecture);
		return;
	}

	if (elf.is_64_bit) {
		k_error("Provided ELF is 64-bit and not supported!");
		return;
	}
}