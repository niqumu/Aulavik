/*====-------------------- elf.h - ELF format parser ---------------------====*\
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

struct elf_prog_header_entry {
#define ELF_PHEADER_TYPE_NULL           0
#define ELF_PHEADER_TYPE_LOADABLE       1
#define ELF_PHEADER_TYPE_DYNAMIC        2
#define ELF_PHEADER_TYPE_INTERPRETER    3
#define ELF_PHEADER_TYPE_NOTE           4
#define ELF_PHEADER_TYPE_PHEADER        6
#define ELF_PHEADER_TYPE_TLS            7
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

struct elf_sect_header_entry {
	char name[16];
	uint32_t name_offset;

#define ELF_SHEADER_TYPE_NULL           0
#define ELF_SHEADER_TYPE_PROGRAM_DATA   1
#define ELF_SHEADER_TYPE_SYMBOL_TABLE   2
#define ELF_SHEADER_TYPE_STRING_TABLE   3
#define ELF_SHEADER_TYPE_RELOC_ADDEND   4
#define ELF_SHEADER_TYPE_BSS            8
#define ELF_SHEADER_TYPE_RELOC          9
	uint32_t type;

#define ELF_SHEADER_FLAG_WRITABLE       0x01
#define ELF_SHEADER_FLAG_ALLOCATE       0x02
#define ELF_SHEADER_FLAG_EXECUTABLE     0x04
#define ELF_SHEADER_FLAG_MERGABLE       0x10
#define ELF_SHEADER_FLAG_STRINGS        0x20
	uint64_t flags;

	uint64_t virtual_addr;
	uint64_t file_offset;
	uint64_t file_size;
	uint64_t alignment;
	uint64_t entry_size;
};

struct elf_sect_header {
	uint16_t entry_count;
	uint16_t entry_size;
	struct elf_sect_header_entry entries[64];
};

struct elf_file {
	bool valid;
	bool is_64_bit;
	bool big_endian;

#define ELF_TARGET_SYSTEM_V     0
	uint8_t target_abi;

#define ELF_TYPE_UNKNOWN        0
#define ELF_TYPE_RELOCATABLE    1
#define ELF_TYPE_EXECUTABLE     2
#define ELF_TYPE_SHARED_OBJ     3
#define ELF_TYPE_CORE           4
	uint16_t type;

#define ELF_ARCH_NONE           0x00
#define ELF_ARCH_X86            0x03
#define ELF_ARCH_X86_64         0x3e
	uint16_t architecture;

	uint64_t addr_entry_point;
	uint64_t addr_prog_header;
	uint64_t addr_sect_header;

	struct elf_prog_header prog_header;
	struct elf_sect_header sect_header;
};

void elf_dump(struct elf_file file);

struct elf_file elf_parse(const uint8_t *buffer, uint32_t size);

#endif /* LOADER_ELF_H */
