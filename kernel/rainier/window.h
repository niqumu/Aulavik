/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
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

#ifndef RAINIER_WINDOW_H
#define RAINIER_WINDOW_H

#include <stdbool.h>
#include <stdint.h>

#include <kernel/graphics/graphics.h>

#define WINDOW_TITLE_THICKNESS  30

#define WINDOW_AREA_NONE        0b00000000
#define WINDOW_AREA_TOP         0b00000001
#define WINDOW_AREA_BOTTOM      0b00000010
#define WINDOW_AREA_LEFT        0b00000100
#define WINDOW_AREA_RIGHT       0b00001000
#define WINDOW_AREA_TITLEBAR    0b00010000
#define WINDOW_AREA_CONTENT     0b00100000
#define WINDOW_AREA_MINIMIZE    0b01000000
#define WINDOW_AREA_CLOSE       0b10000000

struct window {
	bool present;
	char *name;
	int x, y;
	int last_tray_y, last_tray_x;
	uint16_t width, height;
	bool focused;
	bool minimized;

	struct render_context r_ctx;
};

struct window* window_find_front(void);

void window_bring_to_front(struct window *window);

void window_handle_drag(struct window *window, uint8_t flags,
                        int delta_x, int delta_y);

uint8_t window_locate_click(int x, int y, struct window window);

void window_restore(struct window *window);

void window_minimize(struct window *window);

void window_redraw(struct window window);

#endif /* RAINIER_WINDOW_H */
