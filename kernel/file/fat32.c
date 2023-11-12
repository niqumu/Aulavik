/*====----------------------- fat32.c - FAT32 driver ---------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/file/fat32.h>

#include <string.h>

#include <kernel/driver/ata.h>
#include <kernel/logger.h>

struct fat32_drive drive;

void fat32_find_drive(void)
{
	struct ata_device *ata_devices = ata_get_devices();
	struct ata_device candidate;

	for (int i = 0; i < ATA_MAX_DRIVES; i++) {
		candidate = ata_devices[i];

		if (!candidate.present || candidate.type == ATA_TYPE_ATAPI)
			continue;

		/* read the first sector of the drive */
		char data[512];
		ata_read_sectors(i, 0, 1,
		                 (uint8_t *) data);

		/* drive OEM signature position */
		if (!data[3])
			continue;

		memcpy(drive.oem_name, &data[3], 8);
		drive.oem_name[9] = '\0';
		drive.device = candidate;
	}
}

void fat32_init(void)
{
	fat32_find_drive();

	if (!drive.oem_name[0]) {
		k_error("fat: Could not find a FAT formatted hard drive!");
		return;
	}

	k_ok("fat: Found FAT drive, OEM: \"%s\"", drive.oem_name);

	k_ok("Loaded FAT32 driver");
}