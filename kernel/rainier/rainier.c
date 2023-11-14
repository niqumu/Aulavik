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

#include <kernel/graphics/graphics.h>
#include "window.h"

struct render_context *rainier_rctx;

uint16_t mouse_x = 500, mouse_y = 500;

void rainier_process_mouse(struct mouse_packet packet)
{
	mouse_x += packet.delta_x / 2;
	mouse_y += packet.delta_y / 2;
	graphics_rect(0, 0, graphics_get_context()->width, graphics_get_context()->height, background);
	graphics_rect(mouse_x, mouse_y, 5, 5, packet.left_state ? color_1 : color_2);
}

void rainier_main()
{
	rainier_rctx = graphics_get_context();
	graphics_vgradient(0, 0, rainier_rctx->width, rainier_rctx->height,
	                   graphics_color(0x2c5364),
	                   graphics_color(0x0f2027));

	struct window window;
	window.x = 300;
	window.y = 250;
	window.width = 500;
	window.height = 400;
	window.name = "Hello, world!";
//	window_render(window);
}