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
#include <stdlib.h>

struct color c_primary = {237, 168, 112};
struct color c_highlight = {247, 217, 192};
struct color c_shadow = {125, 89, 59};

struct color cbg_primary = {153, 153, 153};
struct color cbg_highlight = {209, 209, 210};
struct color cbg_shadow = {78, 78, 78};

int *drag_dw, *drag_dh;

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
//		rainier_get_windows()[0].focused = false;
//		window_redraw(&rainier_get_windows()[0]);
		rainier_set_focused_window(NULL);

		for (int j = i - 1; j >= 0; j--) {
			rainier_get_windows()[j + 1] = rainier_get_windows()[j];
		}
		
		rainier_get_windows()[0] = w_cpy;
//		rainier_get_windows()[0].focused = true;
//		window_redraw(&rainier_get_windows()[0]);
		rainier_set_focused_window(&rainier_get_windows()[0]);
		return;
	}
}

void window_handle_drag(struct rainier_window *window, uint8_t flags,
                        int delta_x, int delta_y)
{
	if (flags != WINDOW_AREA_NONE && flags != WINDOW_AREA_CONTENT) {
		bool x_scale = flags & WINDOW_AREA_LEFT ||
		               flags & WINDOW_AREA_RIGHT;
		bool y_scale = flags & WINDOW_AREA_TOP ||
		               flags & WINDOW_AREA_BOTTOM;

		if (flags & WINDOW_AREA_TITLEBAR) {
			window->x += delta_x;
			window->y += delta_y;
			return;
		}

		int new_width = window->width;
		int new_height = window->height;

		if (drag_dh == NULL)
			drag_dh = malloc(sizeof(int));

		if (drag_dw == NULL)
			drag_dw = malloc(sizeof(int));

		if (x_scale) {
			if (flags & WINDOW_AREA_LEFT) {
				new_width -= delta_x;
			} else {
				new_width += delta_x;
			}
		}

		if (y_scale) {
			if (flags & WINDOW_AREA_TOP) {
				new_height -= delta_y;
			} else {
				new_height += delta_y;
			}
		}

		window_resize(window, new_width, new_height, drag_dw, drag_dh);

		if (flags & WINDOW_AREA_TOP)
			window->y += *drag_dh;

		if (flags & WINDOW_AREA_LEFT)
			window->x += *drag_dw;
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

static void shaded_rect(struct rainier_window *window, uint32_t x, uint32_t y, uint32_t width,
                        uint32_t height, bool inside)
{
	bool focused = rainier_get_focused_window() == window;
	struct color primary = focused ? c_primary : cbg_primary;
	struct color shadow = focused ? c_shadow : cbg_shadow;
	struct color highlight = focused ? c_highlight : cbg_highlight;

	graphics_rect(&window->ctx, x, y, width, height,
		      inside ? shadow : highlight);
	graphics_rect(&window->ctx, x + 1, y + 1, width - 1,
		      height - 1, inside ? highlight : shadow);
	graphics_rect(&window->ctx, x + 1, y + 1,
		      width - 2, height - 2, primary);
}

void window_redraw_minimized(struct rainier_window *window)
{
	bool focused = rainier_get_focused_window() == window;
	struct color primary = focused ? c_primary : cbg_primary;
	struct color shadow = focused ? c_shadow : cbg_shadow;
	struct color highlight = focused ? c_highlight : cbg_highlight;

	/* primary window outline */
	graphics_rect(&window->ctx, 0, 0, WINDOW_TRAY_WIDTH,
	              WINDOW_TRAY_HEIGHT, highlight);
	graphics_rect(&window->ctx, 2, 2, WINDOW_TRAY_WIDTH - 2,
		      WINDOW_TRAY_HEIGHT - 2, shadow);
	graphics_rect(&window->ctx, 2, 2, WINDOW_TRAY_WIDTH - 4,
		      WINDOW_TRAY_HEIGHT - 4, primary);

	/* inside shading */
	graphics_rect(&window->ctx, 4, 4, WINDOW_TRAY_WIDTH - 8,
		      WINDOW_TRAY_HEIGHT - 8, highlight);
	graphics_rect(&window->ctx, 4, 4, WINDOW_TRAY_WIDTH - 9,
		      WINDOW_TRAY_HEIGHT - 9, shadow);

	/* title bar */
	shaded_rect(window, 5, 5, WINDOW_TRAY_WIDTH - 10,
		    WINDOW_TRAY_HEIGHT - 10, false);
	
	/* text */
	uint16_t string_x = ((WINDOW_TRAY_WIDTH / 2)) -
	                    ((strlen(window->name) * (font_width + FR_KERNING)) / 2);
	fr_render_string(window->ctx, string_x, 11, window->name, graphics_color(0xffffff));
}

void window_redraw(struct rainier_window *window)
{
	if (window->minimized) {
		window_redraw_minimized(window);
		return;
	}

	bool focused = rainier_get_focused_window() == window;
	struct color primary = focused ? c_primary : cbg_primary;
	struct color shadow = focused ? c_shadow : cbg_shadow;
	struct color highlight = focused ? c_highlight : cbg_highlight;

	/* primary window outline */
	graphics_rect(&window->ctx, 0, 0, window->width, window->height, highlight);
	graphics_rect(&window->ctx, 2, 2, window->width - 2, window->height - 2, shadow);
	graphics_rect(&window->ctx, 2, 2, window->width - 4, window->height - 4, primary);

	/* inside shading */
	graphics_rect(&window->ctx, 4, 4, window->width - 8, window->height - 8, highlight);
	graphics_rect(&window->ctx, 4, 4, window->width - 9, window->height - 9, shadow);

	/* title bar */
	shaded_rect(window, 5, 5, window->width - 10, 25, false);

	/* minimize button */
	graphics_rect(&window->ctx, window->width - 30, 6, 1, 24, shadow);
	graphics_rect(&window->ctx, window->width - 29, 6, 1, 24, highlight);
	shaded_rect(window, window->width - 20, 14, 5, 5, false);

	/* close button */
	graphics_rect(&window->ctx, 29, 6, 1, 24, shadow);
	graphics_rect(&window->ctx, 30, 6, 1, 24, highlight);
	shaded_rect(window, 13, 15, 10, 4, false);

	/* text */
	uint16_t string_x = ((window->width / 2)) -
		((strlen(window->name) * (font_width + FR_KERNING)) / 2);
	fr_render_string(window->ctx, string_x, 11, window->name, graphics_color(0xffffff));

	/* draw content */
	graphics_bake_contexts(&window->client_ctx, 0, 0, 5, 30,
	                window->client_ctx.width, window->client_ctx.height, &window->ctx);
}

void window_restore(struct rainier_window *window)
{
	window->minimized = false;
	window->ctx.width = window->width;
	window->ctx.height = window->height;
	window_bring_to_front(window);
}

void window_minimize(struct rainier_window *window)
{
	window->minimized = true;
	window->ctx.width = WINDOW_TRAY_WIDTH;
	window->ctx.height = WINDOW_TRAY_HEIGHT;
	rainier_set_focused_window(window_find_front());
	window_redraw_minimized(window);
}

void window_resize(struct rainier_window *window, int w, int h, int *dw, int *dh)
{
	w = w < WINDOW_MIN_WIDTH ? WINDOW_MIN_WIDTH : w;
	h = h < WINDOW_MIN_HEIGHT ? WINDOW_MIN_HEIGHT : h;

	if (dw != NULL)
		*dw = window->width - w;

	if (dh != NULL)
		*dh = window->height - h;

	window->width = w;
	window->height = h;
	window->ctx.width = window->width;
	window->ctx.height = window->height;
	window->client_ctx.width = window->width - 10;
	window->client_ctx.height = window->height - 35;
	window_redraw(window);
}