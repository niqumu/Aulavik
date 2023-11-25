/*====---------------- vfs_fat.c - FAT VFS implementation ----------------====*\
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

#include "vfs_fat.h"
#include "kernel/logger.h"

#include <stdlib.h>
#include <string.h>

#include <kernel/driver/fat32.h>
#include <kernel/file/vfs.h>

struct fat_file_descriptor fat_open_files[VFS_MAX_OPEN_FILES];

static int fat_next_free_id(void)
{
	for (int i = 0; i < VFS_MAX_OPEN_FILES; i++) {
		if (!fat_open_files[i].present)
			return i;
	}

	return -1; /* no room */
}

int fat_open(const char *path, int flags, ...)
{
	int id = fat_next_free_id();

	if (id == -1)
		return -1; /* max open files reached */

	void *path_cpy_addr = malloc(strlen(path) + 1);
	char *path_cpy = path_cpy_addr;
	strcpy(path_cpy, path);

	/* the next "piece" of the path, either a directory or the file
	 * name itself. i.e., in "0:/foo/bar.png", the parts are "foo" and
	 * "bar.png". */
	char *part = malloc(strlen(path) + 1);

	/* whether we've reached our end destination */
	bool resolved = false;

	struct fat32_directory_entry *entry_buffer =
		malloc(sizeof(struct fat32_directory_entry) * 128);
	uint32_t *count = malloc(sizeof(uint32_t));
	uint32_t current_cluster = fat32_get_drive()->root_cluster;

loop_start:
	memset(part, 0, strlen(path));
	memset(entry_buffer, 0,
	       sizeof(struct fat32_directory_entry) * 128);

	/* store the next part of the path in part */
	for (size_t i = 1; i <= strlen(path); i++) {

		/* if the path keeps going, we haven't reached the end */
		if (path_cpy[i] == '/') {
			memcpy(part, path_cpy, i);
			part[i] = '\0';
			path_cpy += i + 1; /* remove part and '/' */
		}

		/* if the path ends, we should be at the file (resolved) */
		else if (path_cpy[i] == '\0') {
			memcpy(part, path_cpy, i);
			part[i] = '\0';
			path_cpy += i + 1; /* remove part and '/' */
			resolved = true;
		}
	}

	/* read the current directory */
	fat32_read_directory(current_cluster, entry_buffer, count);

	/* search the current directory */
	for (uint32_t i = 0; i < *count; i++) {
		struct fat32_directory_entry entry = entry_buffer[i];

		if (strcmp(entry.display_name, part) == 0) {

			/* this directory is a match, but we still have
			 * more directories to go before the file */
			if (!resolved) {
				current_cluster = entry.first_cluster;
				goto loop_start;
			}

			/* if we should be at the end; the file itself */
			fat_open_files[id].present = true;
			fat_open_files[id].size = entry.size;
			fat_open_files[id].first_cluster = entry.first_cluster;
			strcpy(fat_open_files[id].name, entry.display_name);

			free(path_cpy_addr);
			free(part);
			free(entry_buffer);
			free(count);
			return id;
		}
	}

	/* didn't find the file */
	free(path_cpy_addr);
	free(part);
	free(entry_buffer);
	free(count);
	return -1;
}

int fat_close(int descriptor)
{
	fat_open_files[descriptor].present = false;
	return 0;
}

ssize_t fat_read(int descriptor, void *r_buffer, size_t size)
{
	struct fat_file_descriptor file = fat_open_files[descriptor];

	if (!file.present)
		return -1; /* file wasn't opened first */

	return fat32_read_bytes(file.first_cluster, r_buffer, size);
}

ssize_t fat_write(int descriptor, const void *w_buffer, size_t size)
{
	return 0;
}
