/*====----------------- file_system.h - VFS implementation ---------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/file/file_system.h>

file_system_node_t *root;

uint32_t read_file_fs(file_system_node_t *node, uint32_t offset,
		      uint32_t size, uint8_t *buffer)
{
	if (node->read_file)
		return node->read_file(node, offset, size, buffer);

	return 0;
}

uint32_t write_file_fs(file_system_node_t *node, uint32_t offset,
		       uint32_t size, uint8_t *buffer)
{
	if (node->read_file)
		return node->write_file(node, offset, size, buffer);

	return 0;
}

void open_dir_fs(file_system_node_t *node, uint8_t read, uint8_t write)
{
	if (node->type == NODE_TYPE_DIRECTORY && node->open_dir)
		node->open_dir(node);
}

void close_dir_fs(file_system_node_t *node)
{
	if (node->type == NODE_TYPE_DIRECTORY && node->open_dir)
		node->close_dir(node);
}

dir_entry_t* read_dir_fs(file_system_node_t *node, uint32_t index)
{
	if (node->type == NODE_TYPE_DIRECTORY && node->read_dir)
		return node->read_dir(node, index);

	return 0;
}

file_system_node_t* find_dir_fs(file_system_node_t *node, char *name)
{
	if (node->type == NODE_TYPE_DIRECTORY && node->find_dir)
		return node->find_dir(node, name);

	return 0;
}