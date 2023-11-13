/*====------------------- fat32.h - FAT32 driver header ------------------====*\
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

#ifndef FILE_FAT32_H
#define FILE_FAT32_H

#include <stdint.h>

#include <kernel/driver/ata.h>

#define FAT_CLUSTER_FREE                0x0
#define FAT_CLUSTER_BAD                 0x0ffffff7

/* directory entry attributes bitmap */
#define FAT_ATTRIBUTE_READ_ONLY         0b00000001
#define FAT_ATTRIBUTE_HIDDEN            0b00000010
#define FAT_ATTRIBUTE_SYSTEM            0b00000100
#define FAT_ATTRIBUTE_VOLUME_ID         0b00001000
#define FAT_ATTRIBUTE_DIRECTORY         0b00010000
#define FAT_ATTRIBUTE_ARCHIVE           0b00100000
#define FAT_ATTRIBUTE_DEVICE            0b01000000

struct fat32_date {
	uint16_t year;
	uint8_t month;
	uint8_t day;
};

struct fat32_time {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
};

/* struct representation of a 32-byte entry in a directory table. a directory
 * entry can be either a file or another subdirectory. */
struct fat32_directory_entry {
	char name[9];
	char extension[4];
	char display_name[13];
	uint8_t attributes;
	uint32_t size;  /* size in bytes. dirs/volume labels should have 0 */

	struct fat32_date creation_date;
	struct fat32_date access_date;
	struct fat32_date modify_date;
	struct fat32_time creation_time;
	struct fat32_time modify_time;

	uint32_t first_cluster;  /* this file/dirs first cluster */
};

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
};

void fat32_direntry_dump(struct fat32_directory_entry entry);

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
