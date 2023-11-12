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

#include <kernel/driver/ata.h>

struct fat32_drive {
	struct ata_device device;

	char oem_name[8];
};

void fat32_init(void);

#endif /* FILE_FAT32_H */
