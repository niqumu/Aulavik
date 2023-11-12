/*====------------- fat32_info.c - FAT32 drive information tool ----------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/fat32.h>

#include <kernel/logger.h>

#define BAR     " \e[90m|\e[97m "

void fat32_dump_info(void)
{
	struct fat32_drive drive = *fat32_get_drive();

	if (!drive.present) {
		k_print("\e[91mNo FAT drive present.");
		return;
	}

	k_print("Label: \"%s\", OEM: \"%s\", size: %d MB", drive.label,
		drive.oem_name, (drive.sector_size * drive.sectors) / 1000000);
	k_print("File Allocation Tables: %d", drive.fats);
	k_print("Root entries: %d, root cluster: %d",
		drive.root_entries, drive.root_cluster);
	k_print("Sectors: %d", drive.sectors);
	k_print(BAR "Hidden sectors: %d", drive.hidden_sectors);
	k_print(BAR "Reserved sectors: %d", drive.reserved_sectors);
	k_print(BAR "Sector size: %d bytes", drive.sector_size);
	k_print(BAR "Sectors per track: %d", drive.sectors_per_track);
	k_print(BAR "Sectors per cluster: %d", drive.sectors_per_cluster);
	k_print(BAR "Sectors per FAT: %d", drive.sectors_per_fat);
}