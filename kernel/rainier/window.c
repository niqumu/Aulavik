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
#include "kernel/logger.h"

#include <string.h>

#include <kernel/graphics/graphics.h>
#include <kernel/graphics/font_renderer.h>
#include <kernel/graphics/font.h>

#include <kernel/rainier/rainier.h>

struct color c_primary;
struct color c_highlight;
struct color c_shadow;

void window_bring_to_front(struct window *window)
{
	if (!window->present)
		return;
	
	for (int i = 0; i < 32; i++) {
		if (&rainier_get_windows()[i] != window)
			continue;
		
		struct window w_cpy = *window;
		
		for (int j = i - 1; j >= 0; j--) {
			rainier_get_windows()[j + 1] = rainier_get_windows()[j];
		}
		
		rainier_get_windows()[0] = w_cpy;
		return;
	}
}

void window_handle_drag(struct window *window, uint8_t flags,
			int delta_x, int delta_y)
{
	if (flags != WINDOW_AREA_NONE &&
	    flags != WINDOW_AREA_CONTENT) {
		bool x_scale = flags & WINDOW_AREA_LEFT ||
		               flags & WINDOW_AREA_RIGHT;
		bool y_scale = flags & WINDOW_AREA_TOP ||
		               flags & WINDOW_AREA_BOTTOM;
		bool dirty = false;

//		if (window->width <= 400) {
//			bool stretching_right = flags & WINDOW_AREA_RIGHT && delta_x > 0;
//			bool stretching_left = flags & WINDOW_AREA_LEFT && delta_x < 0;
//			x_scale = stretching_left || stretching_right;
//			dirty = true;
//			window->width = 400;
//		}
//
//		if (window->height <= 300) {
//			bool stretching_down = flags & WINDOW_AREA_BOTTOM && delta_y > 0;
//			bool stretching_up = flags & WINDOW_AREA_TOP && delta_y < 0;
//			y_scale = stretching_up || stretching_down;
//			dirty = true;
//			window->height = 300;
//		}

		if (flags & WINDOW_AREA_TITLEBAR) {
			window->x += delta_x;
			window->y += delta_y;
		}

		if (x_scale) {
			dirty = true;
			if (flags & WINDOW_AREA_LEFT) {
				window->width -= delta_x;
				window->x += delta_x;
			} else {
				window->width += delta_x;
			}
		}

		if (y_scale) {
			dirty = true;
			if (flags & WINDOW_AREA_TOP) {
				window->height -= delta_y;
				window->y += delta_y;
			} else {
				window->height += delta_y;
			}
		}

		window->r_ctx.height = window->height;
		window->r_ctx.width = window->width;

		if (dirty)
			window_render(*window);
	}
}

uint8_t window_locate_click(int x, int y, struct window window)
{
	x -= window.x;
	y -= window.y;

	if (x < -10 || y < -10 || x > window.width + 10 ||
			y > window.height + 10)
		return WINDOW_AREA_NONE;

	uint8_t area = 0;

	if (y < 10) area |= WINDOW_AREA_TOP;
	if (y > window.height - 10) area |= WINDOW_AREA_BOTTOM;
	if (x < 10) area |= WINDOW_AREA_LEFT;
	if (x > window.width - 10) area |= WINDOW_AREA_RIGHT;

	if (area == WINDOW_AREA_NONE)
		return y < 30 ? WINDOW_AREA_TITLEBAR : WINDOW_AREA_CONTENT;

	return area;
}

static void shaded_rect(struct window window, uint32_t x, uint32_t y, uint32_t width,
			uint32_t height, bool inside)
{
	graphics_rect(window.r_ctx, x, y, width, height,
		      inside ? c_shadow : c_highlight);
	graphics_rect(window.r_ctx, x + 1, y + 1, width - 1,
		      height - 1, inside ? c_highlight : c_shadow);
	graphics_rect(window.r_ctx, x + 1, y + 1,
		      width - 2, height - 2, c_primary);
}

void window_render(struct window window)
{
	c_primary = graphics_color(0xeda870);
	c_highlight = graphics_color(0xf7d9c0);
	c_shadow = graphics_color(0x7d593b);

	/* background */
	graphics_rect(window.r_ctx, 0, 0, window.width, window.height, graphics_color(0));

	/* primary window outline */
	graphics_rect(window.r_ctx, 0, 0, window.width, window.height, c_highlight);
	graphics_rect(window.r_ctx, 2, 2, window.width - 2, window.height - 2, c_shadow);
	graphics_rect(window.r_ctx, 2, 2, window.width - 4, window.height - 4, c_primary);

	/* primary window shading */
	graphics_rect(window.r_ctx, 4, 4, window.width - 8, window.height - 8, c_highlight);
	graphics_rect(window.r_ctx, 4, 4, window.width - 9, window.height - 9, c_shadow);

	/* title bar inner shading */
//	graphics_rect(window.r_ctx, 5, 5, window.width - 10, 20, c_highlight);
//	graphics_rect(window.r_ctx, 6, 6, window.width - 11, 19, c_shadow);
//
//	/* title bar fill */
//	graphics_rect(window.r_ctx, 6, 6, window.width - 12, 22, c_primary);
	shaded_rect(window, 5, 5, window.width - 10, 25, false);

	/* content sample 0x4c515d */
	graphics_rect(window.r_ctx, 5, 30, window.width - 10, window.height - 35, graphics_color(0x4c515d));

	/* text */
	uint16_t string_x = ((window.width / 2)) -
		((strlen(window.name) * (font_width + FR_KERNING)) / 2);
	fr_render_string(window.r_ctx, string_x, 11, window.name, graphics_color(0xffffff));
}