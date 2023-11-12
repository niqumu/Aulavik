/*====------------------- fat32.h - FAT32 driver header ------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef FILE_FAT32_H
#define FILE_FAT32_H

#include <stdint.h>

#include <kernel/driver/ata.h>

#define FAT_CLUSTER_FREE                0x0
#define FAT_CLUSTER_BAD                 0x0ffffff7

#define FAT_ATTRIBUTE_READ_ONLY         0x01
#define FAT_ATTRIBUTE_HIDDEN            0x02
#define FAT_ATTRIBUTE_SYSTEM            0x04
#define FAT_ATTRIBUTE_VOLUME_ID         0x08
#define FAT_ATTRIBUTE_DIRECTORY         0x10
#define FAT_ATTRIBUTE_ARCHIVE           0x20
#define FAT_ATTRIBUTE_LONG_FILENAME     (0x01 | 0x02 | 0x04 | 0x08)

struct fat32_drive {
	bool present;
	struct ata_device device;

	/* boot record (sector 0) information */
	char oem_name[9];       /* up to whatever formatted the disk */
	char label[12];         /* up to the user */

	uint32_t sectors;
	uint32_t sectors_per_fat;
	uint16_t hidden_sectors;
	uint16_t reserved_sectors;
	uint16_t sector_size;
	uint16_t sectors_per_track;
	uint8_t sectors_per_cluster;

	uint8_t fats;
	uint16_t root_entries;
	uint32_t root_cluster;

	/* extended boot record (sector 1) information */
};

void fat32_dump_info(void);

struct fat32_drive* fat32_get_drive(void);

/**
 * Iterate over ATA devices, searching for a FAT32 formatted drive. If one is
 * found, it is loaded into the "drive" variable of fat32.c
 *
 * Function implemented in fat32_scan.c
 *
 * @return Whether a FAT32 formatted drive was found
 */
bool fat32_scan_drives(void);

void fat32_init(void);

#endif /* FILE_FAT32_H */
