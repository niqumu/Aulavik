/*====--------------------- vfs.c - Virtual Filesystem -------------------====*\
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

#include <kernel/file/vfs.h>

#include <stdlib.h>
#include <string.h>

#include "../../libc/include/stdlib.h"
#include "../../libc/include/string.h"

struct vfs_mountpoint mountpoints[VFS_MOUNTPOINTS];
struct vfs_file_descriptor open_files[VFS_MAX_OPEN_FILES];

char* vfs_get_mountpoint_path(char *path)
{
	if (strlen(path) < 3)
		return NULL; /* path is too short to be valid */

	if (path[0] < '0' || path[0] > '9')
		return path; /* path is malformed, or nothing to do */

	if (path[1] == '/') { /* 0/... */
		path += 2;
		return path;
	}

	else if (path[2] == '/') { /* 00/... */
		path += 3;
		return path;
	}

	return NULL; /* the path isn't valid */
}

struct vfs_mountpoint* vfs_get_mountpoint(const char *path)
{
	if (strlen(path) < 2)
		return NULL; /* path is too short to be valid */

	uint8_t id, digits = 0;

	if (path[0] < '0' || path[0] > '9')
		return NULL; /* path doesn't start with a device id */

	id = path[0] - '0';
	digits++;

	/* we have a second digit in the device id */
	if (path[1] >= '0' && path[1] <= '9') {

		if (strlen(path) < 3)
			return NULL; /* path is too short to be valid */

		id *= 10;
		id += path[1] - '0';
		digits++;
	}

	if (path[digits] != ':' || path[digits + 1] != '/')
		return NULL; /* path either has a 3 digit device id or no / */

	struct vfs_mountpoint *mountpoint = &mountpoints[id];

	if (!mountpoint->present)
		return NULL; /* id doesn't exist/isn't mounted */

	return mountpoint;
}

int vfs_unmount(struct ata_device device)
{
	/* search mountpoints for the given device */
	for (int id = 0; id < VFS_MOUNTPOINTS; id++) {
		struct vfs_mountpoint mountpoint = mountpoints[id];

		/* does this mountpoint match? */
		if (mountpoint.device.channel != device.channel ||
				mountpoint.device.drive != device.drive)
			continue;

		/* zero out the device and return success */
		memset(&mountpoints[id], 0, sizeof(struct vfs_mountpoint));
		return 0;
	}

	return -1; /* device was not mounted in the first place */
}

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
	      char *name, enum vfs_filesystem fs)
{
	if (mountpoints[id].present)
		return -1; /* given id is already taken */

	struct vfs_mountpoint mountpoint;

	mountpoint.device = device;
	mountpoint.type = INTERNAL;

	memcpy(mountpoint.name, name, strlen(device.name));
	mountpoints[id] = mountpoint;
	mountpoints[id].filesystem = fs;
	mountpoints[id].present = true;

	return 0;
}

int vfs_next_free_descriptor(void)
{
	for (int i = 0; i < VFS_MAX_OPEN_FILES; i++) {
		if (!open_files[i].present)
			return i;
	}

	return -1; /* no room */
}

int vfs_next_free_mountpoint(void)
{
	for (int i = 0; i < VFS_MOUNTPOINTS; i++) {
		if (!mountpoints[i].present)
			return i;
	}

	return -1; /* no room */
}

int vfs_open(const char *path, int flags)
{
	struct vfs_mountpoint *mountpoint = vfs_get_mountpoint(path);

	if (mountpoint == NULL)
		return -1; /* something went wrong getting the mountpoint */

	/* get the absolute path relative to the mountpoint.
	 * e.g. 13/dir/file.foo -> dir/file.foo */
	char *path_cpy = malloc(strlen(path) + 1);
	memcpy(path_cpy, path, strlen(path) + 1);
	path_cpy = vfs_get_mountpoint_path(path_cpy);

	int fs_file_id = mountpoint->operations.open(path_cpy, flags);
	free(path_cpy);

	if (fs_file_id == -1)
		return 1; /* something went wrong on the file-system side */

	int descriptor = vfs_next_free_descriptor();

	if (descriptor == -1)
		return 2; /* no more room to open new files */

	// TODO how do we get the name and size from the file system???
	open_files[descriptor].present = true;
	open_files[descriptor].fs_file_id = fs_file_id;
	open_files[descriptor].mountpoint_id = mountpoint->id;

	return descriptor;
}

int vfs_close(int descriptor)
{
	struct vfs_file_descriptor file = open_files[descriptor];
	struct vfs_mountpoint mountpoint = mountpoints[file.mountpoint_id];

	if (!file.present || !mountpoint.present)
		return -1; /* the file isn't open, or its mountpoint is bad */

	int close = mountpoint.operations.close(file.fs_file_id);

	if (close != 0)
		return 1; /* something went wrong on the file-system side */

	open_files[descriptor].present = false;
	return 0;
}

ssize_t vfs_read(int descriptor, void *buffer, size_t size)
{
	struct vfs_file_descriptor file = open_files[descriptor];
	struct vfs_mountpoint mountpoint = mountpoints[file.mountpoint_id];

	if (!file.present || !mountpoint.present)
		return -1; /* the file isn't open, or its mountpoint is bad */

	ssize_t read = mountpoint.operations.read(descriptor, buffer, size);

	if (file.buffer_read_pos + size < file.file_size) {
		file.buffer_read_pos += size;
	} else {
		file.buffer_read_pos = file.file_size;
	}

	return read;
}

void vfs_init(void)
{

}
