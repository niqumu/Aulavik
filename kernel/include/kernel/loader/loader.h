/*====-------------------- loader.h - Program loader ---------------------====*\
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

#ifndef KERNEL_LOADER_H
#define KERNEL_LOADER_H

#include <kernel/driver/fat32.h>

/**
 * Switch to ring 3 and call a function. Implemented in _loader.asm
 * @param callee A pointer to the function to be called from ring 3
 */
extern void loader_jump_ring3(void *callee, int argc, char **argv);

void loader_load(struct fat32_directory_entry file);

#endif /* KERNEL_LOADER_H */
