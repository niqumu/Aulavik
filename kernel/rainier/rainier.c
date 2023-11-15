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

struct window window;

struct window *selected_window = NULL;
enum window_area selected_window_area = NONE;

/* fps */
int fps = 0;
int frames = 0, ticks = 0;

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

	switch (selected_window_area) {
	case TITLEBAR:
		selected_window->x += cursor_x - last_cursor_x;
		selected_window->y += cursor_y - last_cursor_y;
		break;
	case RIGHT:
		selected_window->width += cursor_x - last_cursor_x;
		window_render(window);
		break;
	case BOTTOM:
		selected_window->height += cursor_y - last_cursor_y;
		window_render(window);
		break;
	case LEFT:
		selected_window->width -= cursor_x - last_cursor_x;
		selected_window->x += cursor_x - last_cursor_x;
		window_render(window);
		break;
	case TOP:
		selected_window->height -= cursor_y - last_cursor_y;
		selected_window->y += cursor_y - last_cursor_y;
		window_render(window);
		break;
	}

	/* background */
	graphics_bake_contexts(background_rctx, 0,
			       0, 0, 0,
			       background_rctx.width,
			       background_rctx.height,
			       double_buffer);

//	graphics_rect(double_buffer, 0, 0, 100, 50, background);
//	graphics_rect(double_buffer, 200, 200, 400, 250, color_1);


	/* window */
	graphics_bake_contexts(window_rctx, 0, 0, window.x,
			       window.y, window.width, window.height,
			       double_buffer);

	/* cursor */
	graphics_bake_contexts(background_rctx, last_cursor_x,
	                       last_cursor_y, last_cursor_x,
	                       last_cursor_y, 15, 15,
	                       double_buffer);
	graphics_bake_contexts(cursor_rctx, 0, 0, cursor_x,
			       cursor_y, 15, 15,
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
	mouse_y += packet.delta_y;

	if (packet.left_state) {
		if (selected_window_area == NONE) {
			selected_window_area = window_locate_click(mouse_x,
								   mouse_y, window);
			if (selected_window_area != NONE)
				selected_window = &window;
		}
	} else {
		selected_window_area = NONE;
	}

//	rainier_render();
}

void rainier_main()
{
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
	graphics_rect(cursor_rctx, 0, 0, 15, 15,
	              graphics_color(0x000000));
	graphics_rect(cursor_rctx, 1, 1, 13, 13,
	              graphics_color(0xffffff));

	rainier_render();
	window.x = 300;
	window.y = 250;
	window.width = 500;
	window.height = 400;
	window.name = "Hello, world!";
	window_create(&window_rctx);

	window_rctx = background_rctx;
	window_rctx.framebuffer = malloc(background_rctx.framebuffer_size);
	window_rctx.framebuffer_size = background_rctx.framebuffer_size;
	window_rctx.width = window.width;
	window_rctx.height = window.height;

	window_render(window);
	k_print("Welcome to Rainier!");

	while (true)
		rainier_render();
}