/*====-------------------- loader.c - Program loader ---------------------====*\
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
#include <string.h>
#include "kernel/task/scheduler.h"

bool loader_verify(struct elf_file elf)
{
	if (!elf.valid) {
		k_error("loader: Invalid ELF file!");
		return false;
	}

	if (elf.architecture != ELF_ARCH_X86) {
		k_error("loader: Unsupported architecture %x!", elf.architecture);
		return false;
	}

	if (elf.target_abi != ELF_TARGET_SYSTEM_V) {
		k_error("loader: Unsupported ABI %x!", elf.target_abi);
		return false;
	}

	if (elf.is_64_bit) {
		k_error("loader: Provided ELF is 64-bit and not supported!");
		return false;
	}

	if (elf.type != ELF_TYPE_EXECUTABLE) {
		k_error("loader: Unsupported ELF type %x!", elf.type);
		return false;
	}

	return true;
}

/**
 * Load, parse, and execute an ELF file on disk
 * @param file The file on disk to be executed
 */
void loader_load(struct fat32_directory_entry file)
{
	/* read and load into memory */
	uint8_t *loaded_addr = malloc(file.size + fat32_get_drive()->cluster_size);
	fat32_read_file(file.first_cluster, loaded_addr, NULL);
	k_ok("Loaded executable to %x", loaded_addr);

	struct elf_file elf = elf_parse(loaded_addr, file.size);

//	elf_dump(elf);

	if (!loader_verify(elf))
		return;

	/* sections */
	for (int i = 0; i < elf.sect_header.entry_count; i++) {

		struct elf_sect_header_entry *section =
			&elf.sect_header.entries[i];

		/* allocate memory for .bss */
		if (section->type == ELF_SHEADER_TYPE_BSS) {

			if (!section->file_size)
				continue;

			if (!(section->flags & ELF_SHEADER_FLAG_ALLOCATE))
				continue;

			void *memory = calloc(section->file_size);
			section->file_offset =
				(uint64_t) memory - (uint64_t) loaded_addr;
			k_debug("loader: Allocated %l bytes at %x for section %d",
				section->file_size, (uintptr_t) memory, i);
		}
	}

	/* segments */
	for (int i = 0; i < elf.prog_header.entry_count; i++) {

		struct elf_prog_header_entry *segment =
			&elf.prog_header.entries[i];

		memcpy((void *) segment->virtual_addr,
		       loaded_addr + segment->file_offset, segment->file_size);
		k_debug("loader: Loaded segment %d to %x", i, segment->virtual_addr);
	}

	k_debug("loader: Calling entry point %x", elf.addr_entry_point);


	char *args[] = {file.display_name};
	void (*entry) (int, char*[]) = (void (*) (int, char*[])) elf.addr_entry_point;

	scheduler_spawn(file.display_name, entry, 1, args);

//	entry(1, args);
//	loader_jump_ring3(entry);

//	k_debug("loader: Execution finished");
}
