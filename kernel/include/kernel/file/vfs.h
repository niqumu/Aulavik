/*====----------------- vfs.h - Virtual Filesystem header ----------------====*\
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

#ifndef FILE_VFS_H
#define FILE_VFS_H      1

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#include <kernel/driver/ata.h>

#define VFS_MOUNTPOINTS         99
#define VFS_MAX_OPEN_FILES      32

enum vfs_device_type {
	INTERNAL,
	REMOVABLE
};

enum vfs_filesystem {
	FAT32
};

struct vfs_file_descriptor {
	bool present;
	char *name;
	int fs_file_id;
	uint64_t file_size;

	int mountpoint_id;
	uint64_t buffer_read_pos;
	uint64_t buffer_write_pos;
	char *buffer;
};

/* function pointers to file-system-specific implementations */
struct vfs_operations {
	int (*open) (const char *path, int flags, ...);
	int (*close) (int descriptor);
	ssize_t (*read) (int descriptor, void *r_buffer, size_t size);
	ssize_t (*write) (int descriptor, const void *w_buffer, size_t size);
};

struct vfs_mountpoint {
	bool present;
	char name[40];
	uint8_t id;
	struct ata_device device;
	enum vfs_device_type type;
	enum vfs_filesystem filesystem;
	struct vfs_operations operations;
};

/**
 * Gets the mountpoint from a path. For example, calling this function with
 * the path "2:/foo/bar.bin" will return a pointer to mountpoint 2.
 * @param path A pointer to the path
 * @return A pointer to the mountpoint path belongs to, or NULL if the path is
 *      malformed or otherwise invalid
 */
struct vfs_mountpoint* vfs_get_mountpoint(const char *path);

int vfs_unmount(struct ata_device device);

/**
 * Mounts the provided device to the provided mountpoint. If the mount was
 * successful, zero is returned. A non-zero value indicates failure.
 * @param device The device to mount
 * @param id The id to mount the device as
 * @param name The label of the drive
 * @param fs The filesystem of the drive, used to call the appropriate driver
 * @return Zero if the mount succeeded, otherwise a non-zero value.
 */
int vfs_mount(struct ata_device device, uint8_t id,
              char *name, enum vfs_filesystem fs);

int vfs_open(const char *path, int flags, int mode);

int vfs_close(int descriptor);

ssize_t vfs_read(int descriptor, void *buffer, size_t size);

int vfs_next_free_mountpoint(void);

#endif /* !FILE_VFS_H */
