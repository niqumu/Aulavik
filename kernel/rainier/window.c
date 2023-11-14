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
#include "../../libc/include/string.h"

#include <kernel/graphics/graphics.h>
#include <kernel/graphics/font_renderer.h>
#include <kernel/graphics/font.h>

struct color c_primary;
struct color c_highlight;
struct color c_shadow;

void window_render(struct window window)
{
	c_primary = graphics_color(0xeda870);
	c_highlight = graphics_color(0xf7d9c0);
	c_shadow = graphics_color(0x7d593b);

	/* primary window outline */
	graphics_rect(window.x, window.y, window.width, window.height, c_highlight);
	graphics_rect(window.x + 2, window.y + 2, window.width - 2, window.height - 2, c_shadow);
	graphics_rect(window.x + 2, window.y + 2, window.width - 4, window.height - 4, c_primary);

	/* content inner shading */
	graphics_rect(window.x + 4, window.y + 4, window.width - 9, window.height - 9, c_shadow);
	graphics_rect(window.x + 5, window.y + 5, window.width - 9, window.height - 9, c_highlight);

	/* title bar inner shading */
	graphics_rect(window.x + 4, window.y + 29, window.width - 9, 1, c_shadow);
	graphics_rect(window.x + 4, window.y + 28, window.width - 9, 1, c_highlight);

	/* title bar fill */
	graphics_rect(window.x + 5, window.y + 5, window.width - 10, 23, c_primary);

	/* content sample */
	graphics_rect(window.x + 5, window.y + 30, window.width - 10, window.height - 35, graphics_color(0x4c515d));

	/* text */
	uint16_t string_x = (window.x + (window.width / 2)) -
		((strlen(window.name) * (font_width + FR_KERNING)) / 2);
	fr_render_string(string_x, window.y + 10, window.name, graphics_color(0xffffff));
}