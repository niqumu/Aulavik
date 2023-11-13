/*====----------------------- fat32.c - FAT32 driver ---------------------====*\
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

#include <kernel/driver/fat32.h>

#include <string.h>

#include <kernel/logger.h>

struct fat32_drive drive;

/* the value used in a FAT to indicate the end of a cluster chain */
uint32_t cluster_eoc;

struct fat32_drive* fat32_get_drive(void)
{
	return &drive;
}

struct fat32_date fat32_parse_date(uint16_t datebytes)
{
	struct fat32_date date;
	date.day = (datebytes & 0b11111);
	date.month = (datebytes & 0b111100000) >> 5;
	date.year = (datebytes & 0b1111111000000000) >> 9;
	return date;
}

struct fat32_time fat32_parse_time(uint16_t timebytes)
{
	struct fat32_time time;
	time.seconds = (timebytes & 0b11111) * 2;
	time.minutes = (timebytes & 0b11111100000) >> 5;
	time.hours = (timebytes & 0b1111100000000000) >> 11;
	return time;
}

void fat32_parse_dirtable(const uint8_t *buffer)
{
	for (int i = 0; true; i++) {
		uint16_t base = 32 * i;
		struct fat32_directory_entry entry;

		if (!buffer[base])
			return; /* there are no more entries */

		/* read and clean the file name */
		memcpy(entry.name, &buffer[base + 0x00], 8);
		for (int a = 7; a > 0; a--) {
			if (entry.name[a] != ' ') {
				entry.name[a + 1] = '\0';
				break;
			}
		}

		/* read and clean the file extension */
		memcpy(entry.extension, &buffer[base + 0x08], 3);
		for (int a = 2; a >= 0; a--) {
			if (entry.extension[a] != ' ') {
				entry.extension[a + 1] = '\0';
				break;
			}
			if (!a) /* no extension */
				entry.extension[0] = '\0';
		}

		/* create the display name */
		size_t namelen = strlen(entry.name);
		memcpy(entry.display_name, entry.name, namelen);
		entry.display_name[namelen] = '\0';
		if (entry.extension[0]) {
			size_t extlen = strlen(entry.extension);
			entry.display_name[namelen] = '.';
			memcpy(&entry.display_name[namelen + 1],
			       entry.extension, extlen);
			entry.display_name[namelen + extlen + 1] = '\0';
		}

		entry.attributes = buffer[base + 0x0b];

		uint16_t creation_time = buffer[base + 0x0e];
		creation_time |= ((uint16_t) buffer[base + 0x0f]) << 8;
		entry.creation_time = fat32_parse_time(creation_time);

		uint16_t creation_date = buffer[base + 0x10];
		creation_date |= ((uint16_t) buffer[base + 0x11]) << 8;
		entry.creation_date = fat32_parse_date(creation_date);

		uint16_t access_date = buffer[base + 0x12];
		access_date |= ((uint16_t) buffer[base + 0x13]) << 8;
		entry.access_date = fat32_parse_date(access_date);

		uint16_t modify_time = buffer[base + 0x16];
		modify_time |= ((uint16_t) buffer[base + 0x17]) << 8;
		entry.modify_time = fat32_parse_time(modify_time);

		uint16_t modify_date = buffer[base + 0x18];
		modify_date |= ((uint16_t) buffer[base + 0x19]) << 8;
		entry.modify_date = fat32_parse_date(modify_date);

		entry.first_cluster = buffer[base + 0x1a];
		entry.first_cluster |= ((uint32_t) buffer[base + 0x1b]) << 8;
		entry.first_cluster |= ((uint32_t) buffer[base + 0x14]) << 16;
		entry.first_cluster |= ((uint32_t) buffer[base + 0x15]) << 24;

		entry.size = buffer[base + 0x1c];
		entry.size |= ((uint16_t) buffer[base + 0x1d]) << 8;

		k_print("");
		fat32_direntry_dump(entry);
		k_print("");
	}
}

// buffer must be at least (drive.sector_size * drive.sectors_per_cluster) big
void fat32_read_cluster(uint32_t cluster, uint8_t *buffer)
{
	/* starting sector of the fat data region */
	/* all clusters are aligned to sectors, which makes this
	 * process magnitudes easier */
	uint32_t data_region_start = (drive.reserved_sectors +
              (drive.fats * drive.sectors_per_fat));

	/* how many sectors into the data region the cluster starts at */
	/* clusters start at 2, hence the subtraction */
	uint32_t data_region_offset = (cluster - 2) * drive.sectors_per_cluster;

	/* final sector of the cluster */
	uint32_t sector = data_region_start + data_region_offset;

	ata_read_sectors(drive.device, sector,
			 drive.sectors_per_cluster, buffer);
}

uint32_t fat32_get_table_entry(uint8_t table, uint32_t cluster)
{
	uint16_t base_sector = drive.reserved_sectors +
	                       (drive.sectors_per_fat * table);
	uint32_t sector = (cluster * 4) / drive.sector_size;
	uint32_t offset = (cluster * 4) % drive.sector_size;
	uint8_t buffer[drive.sector_size];
	ata_read_sectors(drive.device, base_sector + sector, 1, buffer);

	uint32_t entry = buffer[offset];
	entry |= ((uint32_t) buffer[offset + 1]) << 8;
	entry |= ((uint32_t) buffer[offset + 2]) << 16;
	entry |= ((uint32_t) buffer[offset + 3]) << 24;
	return entry & 0x0fffffff;
}

void fat32_init(void)
{
	if (!fat32_scan_drives()) {
		k_error("fat: Could not find a FAT formatted hard drive!");
		return;
	}

	/* the value that is being used to indicate the end of a cluster 
	 * chain (end of chain) is stored in cluster one of the table */
	cluster_eoc = fat32_get_table_entry(0, 1);

	uint8_t root_directory[512];
	fat32_read_cluster(drive.root_cluster, root_directory);

	fat32_parse_dirtable(root_directory);

	k_ok("Loaded FAT32 driver, drive label: \"%s\"", drive.label);
}