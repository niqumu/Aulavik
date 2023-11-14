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

#include "window.h"

#include <string.h>

#include <kernel/graphics/graphics.h>
#include <kernel/graphics/font_renderer.h>
#include <kernel/graphics/font.h>

struct color c_primary;
struct color c_highlight;
struct color c_shadow;

struct render_context *window_ctx;

enum window_area window_locate_click(int x, int y, struct window window)
{
	x -= window.x;
	y -= window.y;

	if (x < -5 || y < -5 || x > window.width + 5 ||
			y > window.height + 5)
		return NONE;

	if (y < 5)
		return TOP;

	if (y > window.height - 5)
		return BOTTOM;

	if (x < 5)
		return LEFT;

	if (x > window.width - 5)
		return RIGHT;

	if (y < WINDOW_TITLE_THICKNESS)
		return TITLEBAR;

	return NONE;
}

void window_render(struct window window)
{
	c_primary = graphics_color(0xeda870);
	c_highlight = graphics_color(0xf7d9c0);
	c_shadow = graphics_color(0x7d593b);

	/* primary window outline */
	graphics_rect(*window_ctx, 0, 0, window.width, window.height, c_highlight);
	graphics_rect(*window_ctx, 2, 2, window.width - 2, window.height - 2, c_shadow);
	graphics_rect(*window_ctx, 2, 2, window.width - 4, window.height - 4, c_primary);

	/* content inner shading */
	graphics_rect(*window_ctx, 4, 4, window.width - 9, window.height - 9, c_shadow);
	graphics_rect(*window_ctx, 5, 5, window.width - 9, window.height - 9, c_highlight);

	/* title bar inner shading */
	graphics_rect(*window_ctx, 4, 29, window.width - 9, 1, c_shadow);
	graphics_rect(*window_ctx, 4, 28, window.width - 9, 1, c_highlight);

	/* title bar fill */
	graphics_rect(*window_ctx, 5, 5, window.width - 10, 23, c_primary);

	/* content sample */
	graphics_rect(*window_ctx, 5, 30, window.width - 10, window.height - 35, graphics_color(0x4c515d));

	/* text */
	uint16_t string_x = ((window.width / 2)) -
		((strlen(window.name) * (font_width + FR_KERNING)) / 2);
	fr_render_string(*window_ctx, string_x, 10, window.name, graphics_color(0xffffff));
}

void window_create(struct render_context *ctx)
{
	window_ctx = ctx;
}