/*====------------- file_system.h - VFS implementation header ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h> /* uint8_t, uint32_t */

#ifndef _KERNEL_FILE_SYSTEM_H
#define _KERNEL_FILE_SYSTEM_H

/* file system node types */
#define NODE_TYPE_FILE          0x1
#define NODE_TYPE_DIRECTORY     0x2
#define NODE_TYPE_SYMLINK       0x3
#define NODE_TYPE_MOUNTPOINT    0x4
#define NODE_TYPE_PIPE          0x5

struct file_system_node;

/* directory entry representation */
typedef struct dir_entry
{
	char name[256];
	uint32_t inode;
} dir_entry_t;

/* posix-style function pointer for file read */
typedef uint32_t (*read_type_t) (struct file_system_node* node,
        uint32_t offset, uint32_t size, uint8_t* buffer);

/* posix-style function pointer for file write */
typedef uint32_t (*write_type_t) (struct file_system_node* node,
        uint32_t offset, uint32_t size, uint8_t* buffer);

/* posix-style function pointer for directory open */
typedef void (*open_type_t) (struct file_system_node* node);

/* posix-style function pointer for directory close */
typedef void (*close_type_t) (struct file_system_node* node);

typedef struct dir_entry * (*read_dir_type_t) (struct file_system_node*,
        uint32_t);

typedef struct file_system_node * (*find_dir_type_t) (struct file_system_node*,
        char *name);

/* semi-posix compliant file system node representation */
typedef struct file_system_node
{
	char name[256];
	uint8_t type;
	uint32_t size;
	uint32_t permissions;
	uint32_t inode;

	/* file/directory operation function pointers */
	read_type_t     read_file;
	write_type_t    write_file;
	open_type_t     open_dir;
	close_type_t    close_dir;
	read_dir_type_t read_dir;
	find_dir_type_t find_dir;

	/* pointer to target for symlink/mount point */
	struct file_system_node_t *symlink;
} file_system_node_t;

uint32_t read_file_fs(file_system_node_t *node, uint32_t offset,
		      uint32_t size, uint8_t *buffer);

uint32_t write_file_fs(file_system_node_t *node, uint32_t offset,
		       uint32_t size, uint8_t *buffer);

void open_dir_fs(file_system_node_t *node, uint8_t read, uint8_t write);

void close_dir_fs(file_system_node_t *node);

dir_entry_t *read_dir_fs(file_system_node_t *node, uint32_t index);

file_system_node_t *find_dir_fs(file_system_node_t *node, char *name);

#endif /* _KERNEL_FILE_SYSTEM_H */