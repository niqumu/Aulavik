/*====---------------- rainier.c - Rainier window manager ----------------====*\
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

#include <kernel/rainier/rainier.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kernel/graphics/font_renderer.h>
#include <kernel/graphics/graphics.h>
#include "window.h"
#include "kernel/logger.h"

struct render_context cursor_rctx;

struct render_context double_buffer;

int mouse_x = 500, mouse_y = 500;
int cursor_x = 500, cursor_y = 500;
int last_cursor_x = 500, last_cursor_y = 500;
bool last_left_state; /* todo move this stuff to mouse driver */

/* windows are stored in a double linked list */
struct rainier_window *top_window = NULL;
struct rainier_window *back_window = NULL;

struct rainier_window *focused_window = NULL;
uint8_t last_click_flags = WINDOW_AREA_NONE;
bool dragging = false;

/* fps */
int fps = 0;
int frames = 0, ticks = 0;

struct rainier_window* rainier_get_focused_window(void)
{
	return focused_window;
}

struct rainier_window* rainier_get_top_window(void)
{
	return top_window;
}

void rainier_set_top_window(struct rainier_window *window)
{
	top_window = window;
}

struct rainier_window* rainier_get_back_window(void)
{
	return back_window;
}

void rainier_set_back_window(struct rainier_window *window)
{
	back_window = window;
}

void rainier_set_focused_window(struct rainier_window *window)
{
	if (window == focused_window)
		return;

	struct rainier_window *old = focused_window;
	focused_window = window;

	if (old != NULL)
		window_redraw_later(old);

	if (window != NULL)
		window_redraw_later(window);
}

struct render_context* rainier_get_buffer(void)
{
	return &double_buffer;
}

void rainier_tick()
{
#ifdef RAINIER_DEBUGGING_ELEMENTS
	ticks++;

	if (ticks >= 20) {
		fps = frames;
		ticks = 0;
		frames = 0;
	}
#endif /* RAINIER_DEBUGGING_ELEMENTS */
}

inline void rainier_render_windows(void)
{
	int minimized_count = 0;
	struct rainier_window *window = back_window;
	
	while (true) {
		if (window->awaiting_redraw)
			window_redraw(window);

		if (window->minimized) {
			graphics_bake_contexts(&window->ctx, 0, 0,
			                       30 + minimized_count * 280,
			                       double_buffer.height - 65,
			                       WINDOW_TRAY_WIDTH,
			                       WINDOW_TRAY_HEIGHT,
			                       &double_buffer);

			window->last_tray_x = 30 + minimized_count * 280;
			window->last_tray_y = double_buffer.height - 65;
			minimized_count++;
		} else {

			/* draw the window normally */
			graphics_bake_contexts(&window->ctx, 0, 0, window->x,
			                       window->y, window->width,
			                       window->height, &double_buffer);
		}

		if (!window->last_window)
			break;
		
		window = window->last_window;
	}
}

void rainier_render_debug(void)
{
#ifdef RAINIER_DEBUGGING_ELEMENTS
	char fps_str[16];
	memset(fps_str, 0, 5);
	sprintf(fps_str, "FPS: %d", fps);
	fr_render_string(double_buffer, 0, 0, fps_str, color_15);
	frames++;
#endif /* RAINIER_DEBUGGING_ELEMENTS */
}

void rainier_render(void)
{
	cursor_x = mouse_x;
	cursor_y = mouse_y;

	if (dragging) {
		window_handle_drag(focused_window, last_click_flags,
		                   cursor_x - last_cursor_x,
		                   cursor_y - last_cursor_y);
	}

	/* windows */
	rainier_render_windows();

	/* cursor */
	graphics_bake_contexts(&cursor_rctx, 0, 0, cursor_x, cursor_y,
		cursor_rctx.width, cursor_rctx.height, &double_buffer);

#ifdef RAINIER_DEBUGGING_ELEMENTS
	rainier_render_debug();
#endif /* RAINIER_DEBUGGING_ELEMENTS */

	/* copy our final image to vram */
	memcpy(graphics_get_global_rctx()->framebuffer,
	       double_buffer.framebuffer, double_buffer.framebuffer_size);

	last_cursor_x = cursor_x;
	last_cursor_y = cursor_y;
}

void rainier_process_mouse(struct mouse_packet packet)
{
	mouse_x += packet.delta_x;
	mouse_x = mouse_x < 0 ? 0 : (mouse_x > (int) double_buffer.width ?
		(int) double_buffer.width : mouse_x);
	mouse_y += packet.delta_y;
	mouse_y = mouse_y < 0 ? 0 : (mouse_y > (int) double_buffer.height ?
		(int) double_buffer.height : mouse_y);

	if (packet.left_state) {

		/* it's not a left click event if already clicking */
		if (last_left_state)
			return;

		last_left_state = true;

		if (dragging)
			return;

		struct rainier_window *ptr = top_window;

		/* iterate over all windows */
		while (true) {

			last_click_flags = window_locate_click(mouse_x,
			                                       mouse_y, *ptr);

			if (last_click_flags == WINDOW_AREA_MINIMIZE) {
				window_minimize(ptr);
				return;
			}

			if (last_click_flags != WINDOW_AREA_NONE) {

				if (ptr->minimized) {
					window_restore(ptr);
					return;
				}

				/* only allow resizing if the window was
				 * already the focused window */
				if (ptr != focused_window) {
					if (last_click_flags == WINDOW_AREA_TITLEBAR ||
					    last_click_flags == WINDOW_AREA_TOP) {
						last_click_flags = WINDOW_AREA_TITLEBAR;
						dragging = true;
					} else {
						dragging = false;
					}
				} else {
					dragging = true;
				}

				window_bring_to_front(ptr);
				return;
			}

			if (!ptr->next_window)
				break;

			ptr = ptr->next_window;
		}
		
		/* the click was not on any window */
		last_click_flags = WINDOW_AREA_NONE;
		dragging = false;
		rainier_set_focused_window(NULL);
	} else {
		last_click_flags = WINDOW_AREA_NONE;
		dragging = false;
		last_left_state = false;
	}
}

void rainier_add_window(struct rainier_window window)
{
	struct rainier_window *ptr = malloc(sizeof(window));
	memcpy(ptr, &window, sizeof(struct rainier_window));

	rainier_set_focused_window(ptr);

	/* zero windows in the list, special case */
	if (top_window == NULL && back_window == NULL) {
		ptr->next_window = NULL;
		ptr->last_window = NULL;
		top_window = ptr;
		back_window = ptr;
		return;
	}

	/* one window in the list, special case */
	if (top_window == back_window) {
		back_window->last_window = ptr;
		back_window->next_window = NULL;
		ptr->next_window = back_window;
		ptr->last_window = NULL;
		top_window = ptr;
		return;
	}

	ptr->last_window = NULL;
	ptr->next_window = top_window;
	top_window->last_window = ptr;

	top_window = ptr;
}

void rainier_destroy_window(struct rainier_window *window)
{
	struct rainier_window *ptr = top_window;

	while (true) {

		if (ptr->next_window == window) {
			ptr->next_window = window->next_window;
			window_destroy(window);
			free(window);
			return;
		}

		if (!ptr->next_window)
			return;

		ptr = ptr->next_window;
	}
}

void rainier_main()
{
	/* set up the main framebuffer */
	double_buffer = *graphics_get_global_rctx();
	double_buffer.framebuffer = malloc(double_buffer.framebuffer_size);

	/* add the desktop window */
	struct rainier_window desktop_window = window_create("Desktop",
		(int) double_buffer.width, (int) double_buffer.height);
	desktop_window.x = 0;
	desktop_window.y = 0;
	window_set_flags(&desktop_window, WINDOW_FLAG_NO_BORDER |
		WINDOW_FLAG_NO_TITLE | WINDOW_FLAG_ANCHOR_BACK);
	graphics_vgradient(&desktop_window.client_ctx, 0, 0, double_buffer.width,
                   double_buffer.height, graphics_color(0x2c5364),
                   graphics_color(0x0f2027));
	rainier_add_window(desktop_window);

	/* set up the cursor context */
	cursor_rctx = double_buffer;
	cursor_rctx.framebuffer = malloc(20 * 27 * 3);
	cursor_rctx.framebuffer_size = 20 * 27 * 3;
	cursor_rctx.width = 20;
	cursor_rctx.height = 27;
	cursor_rctx.blending = true;
	graphics_draw_mcr(&cursor_rctx, cursor1, 0, 0);
	graphics_draw_mcr(&cursor_rctx, cursor2, 0, 0);


	struct rainier_window window1 = window_create("Hello, world!", 500, 400);
	window1.x = 300;
	window1.y = 250;
	graphics_hgradient(&window1.client_ctx, 0, 0,
	                   window1.client_ctx.width, window1.client_ctx.height,
	                   graphics_color_rgb(255, 255, 0),
	                   graphics_color_rgb(0, 255, 255));
	rainier_add_window(window1);

	struct rainier_window window2 = window_create("Rainier", 600, 550);
	window2.x = 400;
	window2.y = 300;
	graphics_vgradient(&window2.client_ctx, 0, 0,
			   window2.client_ctx.width, window2.client_ctx.height,
	                   graphics_color_rgb(255, 0, 0),
			   graphics_color_rgb(0, 0, 255));
	window_set_flags(&window2, WINDOW_FLAG_LOCK_SIZE);
	rainier_add_window(window2);

	while (true)
		rainier_render();
}