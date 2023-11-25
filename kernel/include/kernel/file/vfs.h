/*====----------------- vfs.h - Virtual Filesystem header ----------------====*\
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

#ifndef FILE_VFS_H
#define FILE_VFS_H

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
	uint64_t fs_file_id;
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

int vfs_open(const char *path, int flags);

int vfs_close(int descriptor);

ssize_t vfs_read(int descriptor, void *buffer, size_t size);

int vfs_next_free_mountpoint(void);

void vfs_init(void);

#endif /* FILE_VFS_H */
