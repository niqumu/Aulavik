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

struct render_context background_rctx;
struct render_context cursor_rctx;

struct render_context double_buffer;

int mouse_x = 500, mouse_y = 500;
int cursor_x = 500, cursor_y = 500;
int last_cursor_x = 500, last_cursor_y = 500;
bool last_left_state; /* todo move this stuff to mouse driver */

struct rainier_window windows[32];

struct rainier_window *focused_window = NULL;
uint8_t last_click_flags = WINDOW_AREA_NONE;
bool dragging = false;

/* fps */
int fps = 0;
int frames = 0, ticks = 0;

struct rainier_window* rainier_get_windows(void)
{
	return windows;
}

struct rainier_window* rainier_get_focused_window(void)
{
	return focused_window;
}

void rainier_set_focused_window(struct rainier_window *window)
{
	struct rainier_window *old = focused_window;
	focused_window = window;

	if (old != NULL)
		window_redraw(old);

	if (window != NULL)
		window_redraw(window);
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
	for (int i = 31; i >= 0; i--) {
		if (!windows[i].present)
			continue;

		if (windows[i].minimized) {
			graphics_bake_contexts(&windows[i].ctx, 0, 0,
					       30 + minimized_count * 280,
			                       double_buffer.height - 65,
			                       WINDOW_TRAY_WIDTH,
			                       WINDOW_TRAY_HEIGHT,
			                       &double_buffer);

			windows[i].last_tray_x = 30 + minimized_count * 280;
			windows[i].last_tray_y = double_buffer.height - 65;
			minimized_count++;
			continue;
		}

		/* draw the window normally */
		graphics_bake_contexts(&windows[i].ctx, 0, 0, windows[i].x,
		                       windows[i].y, windows[i].width,
		                       windows[i].height, &double_buffer);
	}
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

	/* background */
	graphics_bake_contexts(&background_rctx, 0,
			       0, 0, 0,
			       background_rctx.width,
			       background_rctx.height,
			       &double_buffer);

	/* windows */
	rainier_render_windows();

	/* cursor */
	graphics_bake_contexts(&cursor_rctx, 0, 0, cursor_x,
			       cursor_y, cursor_rctx.width, cursor_rctx.height,
			       &double_buffer);

	/* fps counter */
#ifdef RAINIER_DEBUGGING_ELEMENTS
	char fps_str[5];
	memset(fps_str, 0, 5);
	sprintf(fps_str, "FPS: %d", fps);
	fr_render_string(double_buffer, 0, 0, fps_str, color_15);
	frames++;
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
	mouse_x = mouse_x < 0 ? 0 : (mouse_x > (int) background_rctx.width ?
		(int) background_rctx.width : mouse_x);
	mouse_y += packet.delta_y;
	mouse_y = mouse_y < 0 ? 0 : (mouse_y > (int) background_rctx.height ?
		(int) background_rctx.height : mouse_y);

	if (packet.left_state) {

		/* it's not a left click event if already clicking */
		if (last_left_state)
			return;

		last_left_state = true;

		if (dragging)
			return;

		/* iterate over all windows */
		for (int i = 0; i < 32; i++) {
			last_click_flags = window_locate_click(mouse_x,
			                                       mouse_y, windows[i]);

			if (last_click_flags == WINDOW_AREA_MINIMIZE) {
				window_minimize(&windows[i]);
				return;
			}

			if (last_click_flags != WINDOW_AREA_NONE) {

				if (windows[i].minimized) {
					window_restore(&windows[i]);
					return;
				}

				/* only allow resizing if the window was
				 * already the focused window */
				if (&windows[i] != focused_window) {
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

				window_bring_to_front(&windows[i]);
				return;
			}
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

void rainier_main()
{
	memset(windows, 0, sizeof(struct rainier_window) * 32);

	background_rctx = *graphics_get_global_rctx();
	background_rctx.framebuffer = malloc(background_rctx.framebuffer_size);

	double_buffer = *graphics_get_global_rctx();
	double_buffer.framebuffer = malloc(background_rctx.framebuffer_size);

	graphics_vgradient(&background_rctx, 0, 0, background_rctx.width,
	                   background_rctx.height,graphics_color(0x2c5364),
	                   graphics_color(0x0f2027));

	cursor_rctx = background_rctx;
	cursor_rctx.framebuffer = malloc(32 * 32);
	cursor_rctx.framebuffer_size = 32 * 32;
	cursor_rctx.width = 20;
	cursor_rctx.height = 27;
	cursor_rctx.blending = true;
	graphics_draw_mcr(&cursor_rctx, cursor1, 0, 0);
	graphics_draw_mcr(&cursor_rctx, cursor2, 0, 0);


	struct rainier_window window1;
	window1.present = true;
	window1.minimized = false;
	window1.x = 300;
	window1.y = 250;
	window1.name = "Hello, world!";
	window1.ctx = background_rctx;
	window1.ctx.framebuffer = malloc(background_rctx.framebuffer_size);
	window1.ctx.framebuffer_size = background_rctx.framebuffer_size;
	window1.client_ctx = background_rctx;
	window1.client_ctx.framebuffer = malloc(background_rctx.framebuffer_size);
	window1.client_ctx.framebuffer_size = background_rctx.framebuffer_size;
	window_resize(&window1, 500, 400, NULL, NULL);
	graphics_hgradient(&window1.client_ctx, 0, 0,
	                   window1.client_ctx.width, window1.client_ctx.height,
	                   graphics_color_rgb(255, 255, 0),
	                   graphics_color_rgb(0, 255, 255));
	windows[0] = window1;
	window_redraw(&windows[0]);

	struct rainier_window window2;
	window2.present = true;
	window2.minimized = false;
	window2.x = 400;
	window2.y = 300;
	window2.name = "Rainier";
	window2.ctx = background_rctx;
	window2.ctx.framebuffer = malloc(background_rctx.framebuffer_size);
	window2.ctx.framebuffer_size = background_rctx.framebuffer_size;
	window2.client_ctx = background_rctx;
	window2.client_ctx.framebuffer = malloc(background_rctx.framebuffer_size);
	window2.client_ctx.framebuffer_size = background_rctx.framebuffer_size;
	window_resize(&window2, 600, 550, NULL, NULL);
	graphics_vgradient(&window2.client_ctx, 0, 0,
			   window2.client_ctx.width, window2.client_ctx.height,
	                   graphics_color_rgb(255, 0, 0),
			   graphics_color_rgb(0, 0, 255));

	windows[1] = window2;
	window_redraw(&windows[1]);

	while (true)
		rainier_render();
}