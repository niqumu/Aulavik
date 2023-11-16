/*====----------- font_renderer.h - Framebuffer font renderer ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef GRAPHICS_FONT_RENDERER_H
#define GRAPHICS_FONT_RENDERER_H

#include <stdint.h>

#include <kernel/graphics/graphics.h>

#define FR_LINE_SPACING 3
#define FR_KERNING      2

void fr_render_char(struct render_context ctx, uint32_t x, uint32_t y, char c,
                    struct color color);

void fr_render_string(struct render_context ctx, uint32_t x, uint32_t y,
	              const char *str, struct color c);

void fr_render_string_bg(struct render_context ctx, uint32_t x, uint32_t y,
                         const char *str, struct color fg, struct color bg);

#endif /* GRAPHICS_FONT_RENDERER_H */