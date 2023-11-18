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

#ifndef LOADER_ELF_H
#define LOADER_ELF_H

#include <stdbool.h>
#include <stdint.h>

#define ELF_TYPE_UNKNOWN        0
#define ELF_TYPE_RELOCATABLE    1
#define ELF_TYPE_EXECUTABLE     2
#define ELF_TYPE_SHARED_OBJ     3
#define ELF_TYPE_CORE           4

/* todo there are a lot of these! */
#define ELF_ARCH_NONE           0x00
#define ELF_ARCH_X86            0x03
#define ELF_ARCH_X86_64         0x3e

struct elf_prog_header_entry {
	uint32_t type;
	uint64_t file_offset;
	uint64_t virtual_addr;
	uint64_t physical_addr;
	uint64_t file_size;
	uint64_t memory_size;
	uint64_t alignment;
};

struct elf_prog_header {
	uint16_t entry_count;
	uint16_t entry_size;
	struct elf_prog_header_entry entries[16];
};

struct elf_file {
	bool valid;
	bool is_64_bit;
	bool big_endian;
	uint8_t target_abi;
	uint16_t type;
	uint16_t architecture;

	uint64_t addr_entry_point;
	uint64_t addr_prog_header;
	uint64_t addr_sect_header;

	struct elf_prog_header prog_header;
};

void elf_dump(struct elf_file file);

struct elf_file elf_parse(const uint8_t *buffer, uint32_t size);

#endif /* LOADER_ELF_H */
