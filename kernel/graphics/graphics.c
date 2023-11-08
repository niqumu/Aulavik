/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/graphics/graphics.h>

#include <kernel/logger.h>
#include <kernel/kernel.h>

struct render_context context;

void graphics_plot_pixel(uint32_t x, uint32_t y, struct color color)
{
	uint8_t *pixel = context.framebuffer + (y * context.pitch +
		x * context.pixel_width);

	pixel[0] = color.b;
	pixel[1] = color.g;
	pixel[2] = color.r;
}

void graphics_rect(uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height, struct color color)
{
	uint8_t *pixel = context.framebuffer + (y * context.pitch +
	                                  x * context.pixel_width);
	uint8_t *base_pixel = pixel;

	for (uint32_t y_off = 0; y_off < height; y_off++) {
		for (uint32_t x_off = 0; x_off < width; x_off++) {
			pixel[0] = color.b;
			pixel[1] = color.g;
			pixel[2] = color.r;

			pixel += context.pixel_width;
		}

		pixel = base_pixel;
		pixel += (y_off + 1) * context.pitch;
	}
}

void graphics_init(void)
{
	context.framebuffer = (uint8_t *) mb_info->framebuffer_addr;
	context.width = mb_info->framebuffer_width;
	context.height = mb_info->framebuffer_height;
	context.bpp = mb_info->framebuffer_bpp;
	context.pixel_width = context.bpp / 8;
	context.pitch = mb_info->framebuffer_pitch;

	/* draw the background */
	graphics_rect(0, 0, context.width, context.height, background);
}