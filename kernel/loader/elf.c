/*====-------------------- elf.c - ELF format parser ---------------------====*\
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

#include <kernel/loader/elf.h>

#include <kernel/logger.h>
#include <string.h>

#define print_bool(a, b) k_print(a, b ? "true" : "false");

void elf_dump(struct elf_file file)
{
	k_print("\n ---------- ELF file dump: ---------- \n");
	print_bool("Valid: %s", file.valid);
	print_bool("64-bit: %s", file.is_64_bit);
	print_bool("Big endian: %s", file.big_endian);
	k_print("Target ABI: %x", file.target_abi);
	k_print("ELF file type: %x", file.type);
	k_print("Target arch: %x\n", file.architecture);

	k_print("Addresses:");
	k_print("Entry point: %x", file.addr_entry_point);
	k_print("Program header: %x", file.addr_prog_header);
	k_print("Section header: %x\n", file.addr_sect_header);

	k_print(" ----- Program header: ----- ");
	k_print("Entries: %d, entry size: %x bytes\n",
		file.prog_header.entry_count, file.prog_header.entry_size);

	for (int i = 0; i < file.prog_header.entry_count; i++) {

		struct elf_prog_header_entry *entry =
			&file.prog_header.entries[i];

		k_print("Segment %d:", i);
		k_print("  Type: %d", entry->type);
		k_print("  Offset in file: %x", entry->file_offset);
		k_print("  Virtual addr: %x", entry->virtual_addr);
		k_print("  Physical addr: %x", entry->physical_addr);
		k_print("  Size in file: %x bytes", entry->file_size);
		k_print("  Size in memory: %d bytes", entry->memory_size);
		k_print("  Alignment: %x\n", entry->alignment);
	}

//	k_print("\n ----- Section header: -----");
//	k_print("Entries: %d, entry size: %x bytes\n",
//	        file.sect_header.entry_count, file.sect_header.entry_size);
//
//	for (int i = 0; i < file.sect_header.entry_count; i++) {
//
//		struct elf_sect_header_entry *entry =
//			&file.sect_header.entries[i];
//
//		if (entry->type == ELF_SHEADER_TYPE_NULL)
//			continue;
//
//		k_print("Section %d; \"%s\":", i, entry->name);
//		k_print("  Type: %x", entry->type);
//		k_print("  Flags: %x", entry->flags);
//		k_print("  Virtual addr: %x", entry->virtual_addr);
//		k_print("  Offset in file: %x", entry->file_offset);
//		k_print("  Size in file: %x bytes", entry->file_size);
//		k_print("  Alignment: %x", entry->alignment);
//
//		if (entry->entry_size) {
//			k_print("  Entry size: %x bytes\n", entry->entry_size);
//		} else {
//			k_print("\n");
//		}
//	}
}

uint32_t elf_read_dword(const uint8_t *buffer, uint64_t offset)
{
	return ((uint32_t) buffer[offset + 3]) << 24 |
		((uint32_t) buffer[offset + 2]) << 16 |
		((uint32_t) buffer[offset + 1]) << 8 |
		((uint32_t) buffer[offset + 0]) << 0;
}

uint16_t elf_read_word(const uint8_t *buffer, uint64_t offset)
{
	return ((uint16_t) buffer[offset + 1]) << 8 | buffer[offset];
}

void elf_parse_prog_header(const uint8_t *buffer, struct elf_file *file)
{
	file->prog_header.entry_size = elf_read_word(buffer, 0x2a);
	file->prog_header.entry_count = elf_read_word(buffer, 0x2c);

	for (int i = 0; i < file->prog_header.entry_count; i++) {

		uint32_t base_addr = file->addr_prog_header +
			(i * file->prog_header.entry_size);

		struct elf_prog_header_entry *entry =
			&file->prog_header.entries[i];

		entry->type = elf_read_dword(buffer, base_addr + 0x00);
		entry->file_offset = elf_read_dword(buffer, base_addr + 0x04);
		entry->virtual_addr = elf_read_dword(buffer, base_addr + 0x08);
		entry->physical_addr = elf_read_dword(buffer, base_addr + 0x0c);
		entry->file_size = elf_read_dword(buffer, base_addr + 0x10);
		entry->memory_size = elf_read_dword(buffer, base_addr + 0x14);
		entry->alignment = elf_read_dword(buffer, base_addr + 0x1c);
	}
}

void elf_parse_sect_header(const uint8_t *buffer, struct elf_file *file)
{
	file->sect_header.entry_size = elf_read_word(buffer, 0x2e);
	file->sect_header.entry_count = elf_read_word(buffer, 0x30);
	uint16_t names_index = elf_read_word(buffer, 0x32);

	for (int i = 0; i < file->sect_header.entry_count; i++) {

		uint32_t base_addr = file->addr_sect_header +
		                     (i * file->sect_header.entry_size);

		struct elf_sect_header_entry *entry =
			&file->sect_header.entries[i];

		entry->name_offset = elf_read_dword(buffer, base_addr + 0x00);
		entry->type = elf_read_dword(buffer, base_addr + 0x04);
		entry->flags = elf_read_dword(buffer, base_addr + 0x08);
		entry->virtual_addr = elf_read_dword(buffer, base_addr + 0x0c);
		entry->file_offset = elf_read_dword(buffer, base_addr + 0x10);
		entry->file_size = elf_read_dword(buffer, base_addr + 0x14);
		entry->alignment = elf_read_dword(buffer, base_addr + 0x20);
		entry->entry_size = elf_read_dword(buffer, base_addr + 0x24);
	}

	struct elf_sect_header_entry *names =
		&file->sect_header.entries[names_index];

	for (int i = 0; i < file->sect_header.entry_count; i++) {

		struct elf_sect_header_entry *entry =
			&file->sect_header.entries[i];

		if (entry->name_offset) {
			strcat(entry->name, (char *)
			(&buffer[names->file_offset + entry->name_offset]));
		} else {
			strcat(entry->name, "NULL entry\0");
		}
	}
}

struct elf_file elf_parse(const uint8_t *buffer, uint32_t size)
{
	struct elf_file file = {0};

	if (buffer[0] == 0x7f && buffer[1] == 'E' &&
			buffer[2] == 'L' && buffer[3] == 'F') {
		file.valid = true;
	} else {
		return file;
	}

	file.is_64_bit = buffer[0x04] == 2;
	file.big_endian = buffer[0x05] == 2;
	file.target_abi = buffer[0x07];
	file.type = elf_read_word(buffer, 0x10);
	file.architecture = elf_read_word(buffer, 0x12);

	file.addr_entry_point = elf_read_dword(buffer, 0x18);
	file.addr_prog_header = elf_read_dword(buffer, 0x1c);
	file.addr_sect_header = elf_read_dword(buffer, 0x20);

	elf_parse_prog_header(buffer, &file);
	elf_parse_sect_header(buffer, &file);

	return file;
}