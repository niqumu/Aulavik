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

enum window_area {
	NONE,
	CONTENT,
	TITLEBAR,
	TOP,
	LEFT,
	BOTTOM,
	RIGHT,
	TOPLEFT,
	TOPRIGHT,
	BOTTOMLEFT,
	BOTTOMRIGHT
};

struct window {
	char *name;
	uint16_t x, y;
	uint16_t width, height;
};

enum window_area window_locate_click(int x, int y, struct window window);

void window_create(struct render_context *ctx);

void window_render(struct window window);

#endif /* RAINIER_WINDOW_H */
