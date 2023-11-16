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

struct color cbg_primary;
struct color cbg_highlight;
struct color cbg_shadow;

struct rainier_window* window_find_front(void)
{
	for (int i = 0; i < 32; i++) {
		if (rainier_get_windows()[i].present &&
		                !rainier_get_windows()[i].minimized)
			return &rainier_get_windows()[i];
	}

	return NULL;
}

/**
 * Brings the provided window to the front of the desktop. Doing so will
 * remove the window from the window list, shuffle all windows in front
 * backwards to take its place, and put the window at the front.
 *
 * This function automatically gives focus to the window being brought
 * forward, and takes it away from the window previously in front. Both
 * windows are redrawn.
 *
 * @param window The window to bring to the front
 */
void window_bring_to_front(struct rainier_window *window)
{
	if (!window->present)
		return;

	for (int i = 0; i < 32; i++) {
		if (&rainier_get_windows()[i] != window)
			continue;
		
		struct rainier_window w_cpy = *window;
		rainier_get_windows()[0].focused = false;
		window_redraw(rainier_get_windows()[0]);

		for (int j = i - 1; j >= 0; j--) {
			rainier_get_windows()[j + 1] = rainier_get_windows()[j];
		}
		
		rainier_get_windows()[0] = w_cpy;
		rainier_get_windows()[0].focused = true;
		window_redraw(rainier_get_windows()[0]);
		return;
	}
}

void window_handle_drag(struct rainier_window *window, uint8_t flags,
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

		window->ctx.height = window->height;
		window->ctx.width = window->width;

		if (dirty)
			window_redraw(*window);
	}
}

uint8_t window_locate_click(int x, int y, struct rainier_window window)
{
	if (window.minimized) {
		if (x < window.last_tray_x || x > window.last_tray_x + 250 ||
		                y < window.last_tray_y ||
				y > window.last_tray_y + 35) {
			return WINDOW_AREA_NONE;
		}

		return WINDOW_AREA_TITLEBAR;
	}

	x -= window.x;
	y -= window.y;

	if (x < -10 || y < -10 || x > window.width + 10 ||
			y > window.height + 10)
		return WINDOW_AREA_NONE;

	if (window.minimized)
		return WINDOW_AREA_TITLEBAR;

	if (x > window.width - 30 && y < 30)
		return WINDOW_AREA_MINIMIZE;

	uint8_t area = 0;

	if (y < 5) area |= WINDOW_AREA_TOP;
	if (y > window.height - 10) area |= WINDOW_AREA_BOTTOM;
	if (x < 10) area |= WINDOW_AREA_LEFT;
	if (x > window.width - 10) area |= WINDOW_AREA_RIGHT;

	if (area == WINDOW_AREA_NONE)
		return y < 30 ? WINDOW_AREA_TITLEBAR : WINDOW_AREA_CONTENT;

	return area;
}

static void shaded_rect(struct rainier_window window, uint32_t x, uint32_t y, uint32_t width,
                        uint32_t height, bool inside)
{
	struct color primary = window.focused ? c_primary : cbg_primary;
	struct color shadow = window.focused ? c_shadow : cbg_shadow;
	struct color highlight = window.focused ? c_highlight : cbg_highlight;

	graphics_rect(window.ctx, x, y, width, height,
		      inside ? shadow : highlight);
	graphics_rect(window.ctx, x + 1, y + 1, width - 1,
		      height - 1, inside ? highlight : shadow);
	graphics_rect(window.ctx, x + 1, y + 1,
		      width - 2, height - 2, primary);
}

void window_redraw_minimized(struct rainier_window window)
{
	struct color primary = window.focused ? c_primary : cbg_primary;
	struct color shadow = window.focused ? c_shadow : cbg_shadow;
	struct color highlight = window.focused ? c_highlight : cbg_highlight;

	/* primary window outline */
	graphics_rect(window.ctx, 0, 0, WINDOW_TRAY_WIDTH,
	              WINDOW_TRAY_HEIGHT, highlight);
	graphics_rect(window.ctx, 2, 2, WINDOW_TRAY_WIDTH - 2,
		      WINDOW_TRAY_HEIGHT - 2, shadow);
	graphics_rect(window.ctx, 2, 2, WINDOW_TRAY_WIDTH - 4,
		      WINDOW_TRAY_HEIGHT - 4, primary);

	/* primary window shading */
	graphics_rect(window.ctx, 4, 4, WINDOW_TRAY_WIDTH - 8,
		      WINDOW_TRAY_HEIGHT - 8, highlight);
	graphics_rect(window.ctx, 4, 4, WINDOW_TRAY_WIDTH - 9,
		      WINDOW_TRAY_HEIGHT - 9, shadow);

	/* title bar */
	shaded_rect(window, 5, 5, WINDOW_TRAY_WIDTH - 10,
		    WINDOW_TRAY_HEIGHT - 10, false);
	
	/* text */
	uint16_t string_x = ((WINDOW_TRAY_WIDTH / 2)) -
	                    ((strlen(window.name) * (font_width + FR_KERNING)) / 2);
	fr_render_string(window.ctx, string_x, 11, window.name, graphics_color(0xffffff));
}

void window_redraw(struct rainier_window window)
{
	if (window.minimized) {
		window_redraw_minimized(window);
		return;
	}
	
	c_primary = graphics_color(0xeda870);
	c_highlight = graphics_color(0xf7d9c0);
	c_shadow = graphics_color(0x7d593b);

	cbg_primary = graphics_color(0x999999);
	cbg_highlight = graphics_color(0xd1d1d2);
	cbg_shadow = graphics_color(0x4e4e4e);

	struct color primary = window.focused ? c_primary : cbg_primary;
	struct color shadow = window.focused ? c_shadow : cbg_shadow;
	struct color highlight = window.focused ? c_highlight : cbg_highlight;

	/* primary window outline */
	graphics_rect(window.ctx, 0, 0, window.width, window.height, highlight);
	graphics_rect(window.ctx, 2, 2, window.width - 2, window.height - 2, shadow);
	graphics_rect(window.ctx, 2, 2, window.width - 4, window.height - 4, primary);

	/* inside shading */
	graphics_rect(window.ctx, 4, 4, window.width - 8, window.height - 8, highlight);
	graphics_rect(window.ctx, 4, 4, window.width - 9, window.height - 9, shadow);

	/* title bar */
	shaded_rect(window, 5, 5, window.width - 10, 25, false);

	/* content sample 0x4c515d */
//	graphics_rect(window.ctx, 5, 30, window.width - 10, window.height - 35, graphics_color(0x4c515d));

	/* minimize button */
	graphics_rect(window.ctx, window.width - 30, 6, 1, 24, shadow);
	graphics_rect(window.ctx, window.width - 29, 6, 1, 24, highlight);
	shaded_rect(window, window.width - 20, 14, 5, 5, false);

	/* close button */
	graphics_rect(window.ctx, 30, 6, 1, 24, shadow);
	graphics_rect(window.ctx, 29, 6, 1, 24, highlight);
	shaded_rect(window, 13, 15, 10, 4, false);

	/* text */
	uint16_t string_x = ((window.width / 2)) -
		((strlen(window.name) * (font_width + FR_KERNING)) / 2);
	fr_render_string(window.ctx, string_x, 11, window.name, graphics_color(0xffffff));

	/* draw content */
	graphics_bake_contexts(window.client_ctx, 0, 0, 5, 30,
	                window.width - 10, window.height - 35, window.ctx);
}

void window_restore(struct rainier_window *window)
{
	window->minimized = false;
	window->focused = true;
	window->ctx.width = window->width;
	window->ctx.height = window->height;
	window_redraw(*window);
}

void window_minimize(struct rainier_window *window)
{
	window->minimized = true;
	window->focused = false;
	window->ctx.width = WINDOW_TRAY_WIDTH;
	window->ctx.height = WINDOW_TRAY_HEIGHT;
	window_redraw(*window);
}