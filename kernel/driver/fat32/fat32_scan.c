/*====--------------- fat32_scan.c - FAT32 drive scanning ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that the relevant copyright
 * notice and permission notice shall be included in all copies or substantial
 * portions of this software and all documentation files.
 *
 * Refer to LICENSE for more information. These works are provided with
 * absolutely no warranty.
 *
 * This file contains functions for detecting FAT32 formatted hard drives, as
 * well as reading and parsing their basic metadata. Functions related to the
 * regular access of data on a drive should not be here, only "one time"
 * functions related to reading basic static information.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/fat32.h>

#include <string.h>

#include <kernel/driver/ata.h>
#include "../../../libc/include/string.h"
#include "kernel/logger.h"

/**
 * Read the FAT drive for basic information. All information is stored on
 * the drive as little-endian.
 */
static void fat32_read_drive(void)
{
	struct fat32_drive *drive = fat32_get_drive();

	/* read the first sector of the drive */
	uint8_t boot_record[512];
	ata_read_sectors(drive->device, 0, 1, boot_record);

	/* read the OEM name at 0x03 */
	memcpy(drive->oem_name, &boot_record[0x03], 8);
	drive->oem_name[8] = '\0';

	drive->sector_size = boot_record[0x0b];
	drive->sector_size |= ((uint16_t) boot_record[0x0c]) << 8;

	drive->sectors_per_cluster = boot_record[0x0d];

	drive->reserved_sectors = boot_record[0x0e];
	drive->reserved_sectors |= ((uint16_t) boot_record[0x0f]) << 8;

	drive->fats = boot_record[0x10];

	drive->root_entries = boot_record[0x11];
	drive->root_entries |= ((uint16_t) boot_record[0x12]) << 8;

	drive->sectors = boot_record[0x13];
	drive->sectors |= ((uint16_t) boot_record[0x14]) << 8;

	drive->sectors_per_track = boot_record[0x18];
	drive->sectors_per_track |= ((uint16_t) boot_record[0x19]) << 8;

	drive->hidden_sectors = boot_record[0x1c];
	drive->hidden_sectors |= ((uint32_t) boot_record[0x1d]) << 8;
	drive->hidden_sectors |= ((uint32_t) boot_record[0x1e]) << 16;
	drive->hidden_sectors |= ((uint32_t) boot_record[0x1f]) << 24;

	/* if the drive contains over 65535 sectors (which cannot fit in the
	 * "sectors" entry at 0x13), the value at 0x13 will be zero, and the
	 * sectors count will instead be stored at 0x20 in four bytes. */
	if (!drive->sectors) {
		drive->sectors = boot_record[0x20];
		drive->sectors |= ((uint32_t) boot_record[0x21]) << 8;
		drive->sectors |= ((uint32_t) boot_record[0x22]) << 16;
		drive->sectors |= ((uint32_t) boot_record[0x23]) << 24;
	}

	drive->sectors_per_fat = boot_record[0x24];
	drive->sectors_per_fat |= ((uint32_t) boot_record[0x25]) << 8;
	drive->sectors_per_fat |= ((uint32_t) boot_record[0x26]) << 16;
	drive->sectors_per_fat |= ((uint32_t) boot_record[0x27]) << 24;

	drive->root_cluster = boot_record[0x2c];
	drive->root_cluster |= ((uint32_t) boot_record[0x2d]) << 8;
	drive->root_cluster |= ((uint32_t) boot_record[0x2e]) << 16;
	drive->root_cluster |= ((uint32_t) boot_record[0x2f]) << 24;

	/* read the volume label at 0x43 */
	memcpy(drive->label, &boot_record[0x47], 11);

	/* clean up the volume label, removing trailing spaces */
	for (int i = 10; i > 0; i--) {
		if (drive->label[i] != ' ') {
			drive->label[i + 1] = '\0';
			break;
		}
	}
}

/**
 * Iterate over ATA devices, searching for a FAT32 formatted drive. If one is
 * found, it is loaded into the "drive" variable of fat32.c
 *
 * @return Whether a FAT32 formatted drive was found
 */
bool fat32_scan_drives(void)
{
	struct ata_device *ata_devices = ata_get_devices();
	struct ata_device candidate;

	for (int i = 0; i < ATA_MAX_DRIVES; i++) {
		candidate = ata_devices[i];

		if (!candidate.present || candidate.type == ATA_TYPE_ATAPI)
			continue;

		/* read the first sector of the drive */
		uint8_t boot_record[512];
		ata_read_sectors(candidate, 0, 1, boot_record);

		/* fat drives should start with JUMP (some addr) NOP */
		if (boot_record[0] != 0xeb || boot_record[2] != 0x90)
			continue;

		/* fat32 drives should contain a signature of either 0x28
		 * or 0x29 at byte 0x42 of the boot record */
		if (boot_record[0x42] != 0x28 && boot_record[0x42] != 0x29)
			continue;

		fat32_get_drive()->device = candidate;
		fat32_get_drive()->present = true;
		fat32_read_drive();
		return true;
	}

	return false; /* we didn't find any drive */
}