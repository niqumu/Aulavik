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
struct render_context window_rctx;

struct render_context double_buffer;

int mouse_x = 500, mouse_y = 500;
int cursor_x = 500, cursor_y = 500;
int last_cursor_x = 500, last_cursor_y = 500;

struct window windows[32];

struct window *selected_window = NULL;
uint8_t selected_window_area = WINDOW_AREA_NONE;

/* fps */
int fps = 0;
int frames = 0, ticks = 0;

struct window* rainier_get_windows(void)
{
	return windows;
}

void rainier_tick()
{
	ticks++;

	if (ticks >= 20) {
		fps = frames;
		ticks = 0;
		frames = 0;
//		k_debug("Rendered %d frames last second", fps);
	}
}

void rainier_render()
{
	cursor_x = mouse_x;
	cursor_y = mouse_y;

	if (selected_window != NULL)
		window_handle_drag(selected_window, selected_window_area,
				   cursor_x - last_cursor_x,
				   cursor_y - last_cursor_y);

	/* background */
	graphics_bake_contexts(background_rctx, 0,
			       0, 0, 0,
			       background_rctx.width,
			       background_rctx.height,
			       double_buffer);

//	graphics_rect(double_buffer, 0, 0, 100, 50, background);
//	graphics_rect(double_buffer, 200, 200, 400, 250, color_1);


	/* windows */
	for (int i = 31; i >= 0; i--) {
		if (!windows[i].present)
			continue;

		graphics_bake_contexts(windows[i].r_ctx, 0, 0, windows[i].x,
		                       windows[i].y, windows[i].width,
		                       windows[i].height, double_buffer);
	}

	/* cursor */
	graphics_bake_contexts(background_rctx, last_cursor_x - 7,
	                       last_cursor_y - 7, last_cursor_x - 7,
	                       last_cursor_y - 7, 15, 15,
	                       double_buffer);
	graphics_bake_contexts(cursor_rctx, 0, 0, cursor_x - 7,
			       cursor_y - 7, 15, 15,
			       double_buffer);

	/* fps counter */
	char fps_str[5];
	memset(fps_str, 0, 5);
	sprintf(fps_str, "FPS: %d", fps);
	fr_render_string(double_buffer, 0, 0, fps_str, color_15);

	/* copy our final image to vram */
	memcpy(graphics_get_global_rctx()->framebuffer,
	       double_buffer.framebuffer, double_buffer.framebuffer_size);

	last_cursor_x = cursor_x;
	last_cursor_y = cursor_y;
	frames++;
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

//		/* it's not a left click even if already clicking */
//		if (selected_window_area != WINDOW_AREA_NONE)
//			return;

		for (int i = 0; i < 32; i++) {
			if (selected_window_area == WINDOW_AREA_NONE) {
				selected_window_area = window_locate_click(mouse_x,
			                                mouse_y, windows[i]);
				if (selected_window_area != WINDOW_AREA_NONE) {
					window_bring_to_front(&windows[i]);
					selected_window = &windows[0];
					break;
				}
			}
		}

	} else {
		selected_window_area = WINDOW_AREA_NONE;
		selected_window = NULL;
	}
}

void rainier_main()
{
	memset(windows, 0, sizeof(struct window) * 32);

	background_rctx = *graphics_get_global_rctx();
	background_rctx.framebuffer = malloc(background_rctx.framebuffer_size);

	double_buffer = *graphics_get_global_rctx();
	double_buffer.framebuffer = malloc(background_rctx.framebuffer_size);

	graphics_vgradient(background_rctx, 0, 0, background_rctx.width,
	                   background_rctx.height,graphics_color(0x2c5364),
	                   graphics_color(0x0f2027));

	/* render the background to vram once */
	memcpy(graphics_get_global_rctx()->framebuffer,
	       background_rctx.framebuffer, background_rctx.framebuffer_size);

	cursor_rctx = background_rctx;
	cursor_rctx.framebuffer = malloc(15 * 15);
	cursor_rctx.framebuffer_size = 15 * 15;
	cursor_rctx.width = 15;
	cursor_rctx.height = 15;
	graphics_rect(cursor_rctx, 0, 0, 14, 14,
	              graphics_color(0x000000));
	graphics_rect(cursor_rctx, 1, 1, 12, 12,
	              graphics_color(0xffffff));


	struct window window1;
	window1.present = true;
	window1.x = 300;
	window1.y = 250;
	window1.width = 500;
	window1.height = 400;
	window1.name = "Hello, world!";
	window1.r_ctx = background_rctx;
	window1.r_ctx.framebuffer = malloc(background_rctx.framebuffer_size);
	window1.r_ctx.framebuffer_size = background_rctx.framebuffer_size;
	window1.r_ctx.width = window1.width;
	window1.r_ctx.height = window1.height;
	windows[0] = window1;
	window_render(window1);

	struct window window2;
	window2.present = true;
	window2.x = 400;
	window2.y = 300;
	window2.width = 600;
	window2.height = 550;
	window2.name = "Daniel";
	window2.r_ctx = background_rctx;
	window2.r_ctx.framebuffer = malloc(background_rctx.framebuffer_size);
	window2.r_ctx.framebuffer_size = background_rctx.framebuffer_size;
	window2.r_ctx.width = window2.width;
	window2.r_ctx.height = window2.height;
	windows[1] = window2;
	window_render(window2);

	while (true)
		rainier_render();
}