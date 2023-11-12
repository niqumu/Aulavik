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

#include <kernel/logger.h>

struct fat32_drive drive;

/* the value used in a FAT to indicate the end of a cluster chain */
uint32_t cluster_eoc;

struct fat32_drive* fat32_get_drive(void)
{
	return &drive;
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
	k_print("\"%s\"", root_directory);

	k_ok("Loaded FAT32 driver, drive label: \"%s\"", drive.label);
}