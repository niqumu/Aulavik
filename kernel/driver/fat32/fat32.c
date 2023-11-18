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

#include <stdlib.h>
#include <string.h>

#include <kernel/logger.h>

struct fat32_drive drive;
struct fat32_directory root_directory = {0};

/* the value used in a FAT to indicate the end of a cluster chain */
uint32_t cluster_eoc;

struct fat32_directory* fat32_get_root(void)
{
	return &root_directory;
}

struct fat32_drive* fat32_get_drive(void)
{
	return &drive;
}

static struct fat32_date fat32_parse_date(uint16_t datebytes)
{
	struct fat32_date date;
	date.day = (datebytes & 0b11111);
	date.month = (datebytes & 0b111100000) >> 5;
	date.year = (datebytes & 0b1111111000000000) >> 9;
	return date;
}

static struct fat32_time fat32_parse_time(uint16_t timebytes)
{
	struct fat32_time time;
	time.seconds = (timebytes & 0b11111) * 2;
	time.minutes = (timebytes & 0b11111100000) >> 5;
	time.hours = (timebytes & 0b1111100000000000) >> 11;
	return time;
}

// buffer must be at least (drive.cluster_size) big
static void fat32_read_cluster(uint32_t cluster, uint8_t *buffer)
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

static uint8_t fat32_parse_directory_cluster(const uint8_t *cluster,
				   struct fat32_directory_entry* dest,
				   uint32_t *count)
{
	/* a directory entry is 32 bytes, therefore a single cluster of
	 * a directory table can store up to (cluster size / 32) entries */
	for (int i = 0; i < drive.cluster_size / 32; i++) {
		uint16_t base = 32 * i;
		struct fat32_directory_entry entry;

		if (!cluster[base])
			return 0; /* there are no more entries */

		if (cluster[base] == 0xe5)
			continue; /* unused entry */

		/* read and clean the file name */
		memcpy(entry.name, &cluster[base + 0x00], 8);
		for (int a = 7; a >= 0; a--) {
			if (entry.name[a] != ' ') {
				entry.name[a + 1] = '\0';
				break;
			}
		}

		/* read and clean the file extension */
		memcpy(entry.extension, &cluster[base + 0x08], 3);
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

		entry.attributes = cluster[base + 0x0b];

		uint16_t creation_time = cluster[base + 0x0e];
		creation_time |= ((uint16_t) cluster[base + 0x0f]) << 8;
		entry.creation_time = fat32_parse_time(creation_time);

		uint16_t creation_date = cluster[base + 0x10];
		creation_date |= ((uint16_t) cluster[base + 0x11]) << 8;
		entry.creation_date = fat32_parse_date(creation_date);

		uint16_t access_date = cluster[base + 0x12];
		access_date |= ((uint16_t) cluster[base + 0x13]) << 8;
		entry.access_date = fat32_parse_date(access_date);

		uint16_t modify_time = cluster[base + 0x16];
		modify_time |= ((uint16_t) cluster[base + 0x17]) << 8;
		entry.modify_time = fat32_parse_time(modify_time);

		uint16_t modify_date = cluster[base + 0x18];
		modify_date |= ((uint16_t) cluster[base + 0x19]) << 8;
		entry.modify_date = fat32_parse_date(modify_date);

		entry.first_cluster = cluster[base + 0x1a];
		entry.first_cluster |= ((uint32_t) cluster[base + 0x1b]) << 8;
		entry.first_cluster |= ((uint32_t) cluster[base + 0x14]) << 16;
		entry.first_cluster |= ((uint32_t) cluster[base + 0x15]) << 24;

		entry.size = cluster[base + 0x1c];
		entry.size |= ((uint16_t) cluster[base + 0x1d]) << 8;

		dest[*count] = entry;
		(*count)++;
	}

	return 1; /* there are still more entries! */
}

/**
 * Read the value stored in the FAT for the given cluster. This value will
 * be the cluster number of the next cluster in the chain, or the value stored
 * in cluster_eoc if the chain is finished.
 *
 * @param cluster The cluster to look up in the FAT
 * @return The value stored in the FAT, aka the next cluster in the chain, or
 *      the value stored in cluster_eoc if the chain is finished.
 */
uint32_t fat32_get_table_entry(uint32_t cluster)
{
	uint8_t table = 1; /* todo figure out when (if ever) to use table 2 */

	/* base location of the table, always aligned to a sector */
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

/**
 * Read a directory into a buffer of directory entry structs, given the
 * starting cluster of the directory table
 *
 * @param start_cluster The first cluster of the directory table
 * @param dest A buffer of directory entries to be populated
 * @param count A pointer to the number of directory entries read
 */
void fat32_read_directory(uint32_t start_cluster,
                          struct fat32_directory_entry* dest, uint32_t *count)
{
	/* there seems to be a bug where first level subdirectories of root
	 * have their ".." (parent directory link) point to cluster 0, rather
	 * than the correct root cluster. this is a temporary fix.
	 *
	 * todo: further investigate this bug */
	if (start_cluster == 0)
		start_cluster = drive.root_cluster;

	uint32_t cluster = start_cluster;
	uint8_t buffer[drive.cluster_size];
	uint32_t *offset;

	if (count != NULL) {
		*count = 0;
		offset = count;
	} else {
		*offset = 0;
	}

	while (true) { /* keep reading clusters until the end */
		memset(buffer, 0, sizeof(buffer));
		fat32_read_cluster(cluster, buffer);

		/* actually read this cluster of the directory table */
		if (!fat32_parse_directory_cluster(buffer, dest, offset))
			return; /* directory table says we reached the end */

		/* get the next cluster in the chain from the FAT */
		cluster = fat32_get_table_entry(cluster);

		if (cluster >= cluster_eoc)
			return; /* FAT says we've reached the end */
	}
}

/**
 * Read a file into a buffer, given the starting cluster of the file. For
 * directory tables, the fat32_read_directory function should be used instead.
 *
 * @param start_cluster The first cluster of the file
 * @param dest A sufficiently large buffer to be read into
 * @param count A pointer to the number of bytes read
 */
void fat32_read_file(uint32_t start_cluster, uint8_t* dest, uint32_t *size)
{
	uint32_t cluster = start_cluster;
	uint8_t buffer[drive.cluster_size];
	uint32_t *offset;

	if (size != NULL) {
		*size = 0;
		offset = size;
	} else {
		*offset = 0;
	}

	while (true) { /* keep reading clusters until the end */
		memset(buffer, 0, sizeof(buffer));

		/* actually read this cluster of the file */
		fat32_read_cluster(cluster, buffer);
		memmove(&dest[*offset], buffer, drive.cluster_size);
		(*offset) += drive.cluster_size;

		/* get the next cluster in the chain from the FAT */
		cluster = fat32_get_table_entry(cluster);

		if (cluster >= cluster_eoc)
			return; /* FAT says we've reached the end */
	}
}

void fat32_init(void)
{
	if (!fat32_scan_drives()) {
		k_error("fat: Could not find a FAT formatted hard drive!");
		return;
	}

	/* the value that is being used to indicate the end of a cluster 
	 * chain (end of chain) is stored in cluster one of the table */
	cluster_eoc = fat32_get_table_entry(0);

	/* read the root directory into memory */
	root_directory.entries = malloc(sizeof(struct fat32_directory_entry) * 128);
	root_directory.name[0] = '/';
	root_directory.name[1] = '\0';
	fat32_read_directory(drive.root_cluster, root_directory.entries,
			     &root_directory.entry_count);

	k_ok("Loaded FAT32 driver, drive label: \"%s\"", drive.label);
}