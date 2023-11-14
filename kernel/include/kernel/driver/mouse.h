/*====------------------- mouse.h - PS/2 mouse driver --------------------====*\
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

#ifndef DRIVER_MOUSE_H
#define DRIVER_MOUSE_H

#include <stdbool.h>

struct mouse_packet {
	int delta_x;
	int delta_y;

	bool left_state;
	bool middle_state;
	bool right_state;
};

void mouse_handle_interrupt(void);

void mouse_tick(void);

void mouse_init(void);

#endif /* DRIVER_MOUSE_H */
