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

struct render_context *rainier_rctx;

void rainier_main()
{
	rainier_rctx = graphics_get_context();
	graphics_vgradient(0, 0, rainier_rctx->width, rainier_rctx->height,
	                   graphics_color(0x2c5364),
	                   graphics_color(0x0f2027));
}