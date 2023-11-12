/*====----------------------- fat32.c - FAT32 driver ---------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/fat32.h>

#include <kernel/logger.h>

struct fat32_drive drive;

/* the value that is used to indicate the end of a cluster chain */
uint32_t cluster_eoc;

struct fat32_drive* fat32_get_drive(void)
{
	return &drive;
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

	k_ok("Loaded FAT32 driver, drive label: \"%s\"", drive.label);
}