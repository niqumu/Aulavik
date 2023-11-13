/*====------------- fat32_info.c - FAT32 drive information tool ----------====*\
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

#include <stdio.h>
#include <string.h>

#include <kernel/logger.h>

#define BAR     " \e[90m|\e[97m "

char* fat32_time_dump(char *buffer, struct fat32_time time)
{
	sprintf(buffer, "%2d:%2d:%2d %s", time.hours % 12, time.minutes,
		time.seconds, time.hours > 12 ? "PM" : "AM");
	return buffer;
}

char* fat32_date_dump(char *buffer, struct fat32_date date)
{
	sprintf(buffer, "%d/%d/%d", date.month, date.day, date.year + 1980);
	return buffer;
}

void fat32_dir_entry_dump(struct fat32_directory_entry entry)
{
	k_print("\"%s\", type: %s, size: %d bytes", entry.display_name,
	        entry.attributes & FAT_ATTRIBUTE_DIRECTORY ? "directory" : "file",
	        entry.size);
	k_print(BAR "First cluster: %d", entry.first_cluster);

	char time_buffer[32] = {0}, date_buffer[32] = {0};

	k_print(BAR "Created at %s on %s", fat32_time_dump(time_buffer,
                entry.creation_time), fat32_date_dump(date_buffer,
                entry.creation_date));
        memset(time_buffer, 0, 32);
	memset(date_buffer, 0, 32);

	k_print(BAR "Modified at %s on %s", fat32_time_dump(time_buffer,
                entry.modify_time), fat32_date_dump(date_buffer,
		entry.modify_date));
	memset(time_buffer, 0, 32);
	memset(date_buffer, 0, 32);

	k_print(BAR "Accessed on %s", fat32_date_dump(date_buffer,
	        entry.access_date));

	k_print("");
}

void fat32_directrory_dump(struct fat32_directory directory)
{
	k_print("Dump of directory \"%s\"", directory.name);
	k_print("Directory has %d entries\n", directory.entry_count);

	for (uint32_t i = 0; i < directory.entry_count; i++) {
		struct fat32_directory_entry entry = directory.entries[i];
		fat32_dir_entry_dump(entry);
	}
}

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