/*====---------------- vfs_fat.h - FAT VFS implementation ----------------====*\
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

#ifndef FILE_VFS_FAT_H
#define FILE_VFS_FAT_H

#include <stdbool.h>
#include <stdint.h>

struct fat_file_descriptor {
	bool present;
	char name[13];
	uint32_t first_cluster;
	uint32_t size;
};

#endif /* FILE_VFS_FAT_H */
