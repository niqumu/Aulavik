/*====----------- font_renderer.c - Framebuffer font renderer ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/graphics/font_renderer.h>

#include <kernel/graphics/font.h>

void fr_render_char(struct render_context ctx, uint32_t x, uint32_t y, char c,
	            struct color color)
{
	if (c < 33) /* no visible ascii characters exist below this */
		return;

	int index = c - 32; /* font starts with space (char 32) */
	uint8_t *character = font_characters[index];
	uint8_t bytes = (font_height * font_width) / 8;

	/* rendering starts at the bottom right */
	uint32_t draw_x = x + font_width;
	uint32_t draw_y = y + font_height;

	/* iterate over each byte in the character */
	for (uint8_t i = 0; i < bytes; i++) {

		uint8_t byte = character[i];

		/* iterate over each bit in the byte */
		for (uint8_t j = 0; j < 8; j++) {
			uint8_t bit = byte & (1 << j);

			if (bit)
				graphics_plot_pixel(ctx, draw_x, draw_y, color);

			if (--draw_x <= x) {
				draw_x = x + font_width;
				draw_y--;
			}
		}
	}
}

void fr_render_string(struct render_context ctx, uint32_t x, uint32_t y,
                      const char *str, struct color c)
{
	for (int i = 0; str[i]; i++) {
		fr_render_char(ctx, x, y, str[i], c);
		x += font_width + FR_KERNING;
	}
}