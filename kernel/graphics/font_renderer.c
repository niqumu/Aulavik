/*====----------- font_renderer.c - Framebuffer font renderer ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/graphics/font_renderer.h>

#include <kernel/logger.h>

#include "font.h"

uint8_t char_width = 8;
uint8_t char_height = 13;

void fr_render_char(uint32_t x, uint32_t y, char c, struct color color)
{
	if (c < 33)
		return;

	int index = c - 32;
	uint8_t *character = characters[index];
	uint8_t height = char_height;
	uint8_t width = char_width;
	uint8_t bytes = (height * width) / 8;

	uint32_t draw_x = x + width, draw_y = y + height + width;

	/* iterate over each byte in the character */
	for (uint8_t i = 0; i < bytes; i++) {

		uint8_t byte = character[i];

		/* iterate over each bit in the byte */
		for (uint8_t j = 0; j < 8; j++) {
			uint8_t bit = byte & (1 << j);

			if (bit)
				graphics_plot_pixel(draw_x, draw_y, color);

			if (--draw_x <= x) {
				draw_x = x + width;
				draw_y--;
			}
		}
	}
}

void fr_render_string(uint32_t x, uint32_t y, const char *str, struct color c)
{
	for (int i = 0; str[i]; i++) {
		fr_render_char(x, 15, str[i], c);
		x += char_width + FR_KERNING;
	}
}