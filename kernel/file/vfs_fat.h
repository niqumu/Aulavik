/*====---------------- vfs_fat.h - FAT VFS implementation ----------------====*\
 *
 * This file is a part of the Aulavik project. The Aulavik project is free
 * software, licenced under the MIT License.
 *
 * Usage of these works (including, yet not limited to, reuse, modification,
 * copying, distribution, and selling) is permitted, provided that the relevant
 * copyright notice and permission notice (as specified in LICENSE) shall be
 * included in all copies or substantial portions of this software and all
 * documentation files.
 *
 * These works are provided "AS IS" with absolutely no warranty of any kind,
 * either expressed or implied.
 *
 * You should have received a copy of the MIT License alongside this software;
 * refer to LICENSE for information. If not, refer to https://mit-license.org.
 *
\*====--------------------------------------------------------------------====*/

#ifndef FILE_VFS_FAT_H
#define FILE_VFS_FAT_H  1

#include <stdbool.h>
#include <stdint.h>

struct fat_file_descriptor {
	bool present;
	char name[13];
	uint32_t first_cluster;
	uint32_t size;
};

int fat_open(const char *path, int flags, ...);

#endif /* !FILE_VFS_FAT_H */
