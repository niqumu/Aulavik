/*====---------------- rainier.h - Rainier window manager ----------------====*\
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

#ifndef RAINIER_H
#define RAINIER_H

#include <kernel/driver/mouse.h>

void rainier_tick();

void rainier_process_mouse(struct mouse_packet packet);

void rainier_main();

#endif /* RAINIER_H */
