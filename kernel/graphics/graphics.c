/*====---------------- graphics.c - Tiny graphics library ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/graphics/graphics.h>

#include <kernel/kernel.h>

struct render_context context;

struct render_context* graphics_get_context(void)
{
	return &context;
}

/* draw a pixel at the specified location, in the specified color. for bulk
 * rendering (e.g. rectangles), the specialized functions are far faster */
void graphics_plot_pixel(uint32_t x, uint32_t y, struct color color)
{
	uint8_t *pixel = context.framebuffer + (y * context.pitch +
		x * context.pixel_width);

	pixel[0] = color.b;
	pixel[1] = color.g;
	pixel[2] = color.r;
}

/* draw a rectangle of the specified dimensions at the specified location,
 * in the specified color */
void graphics_rect(uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height, struct color color)
{
	uint8_t *pixel = context.framebuffer + (y * context.pitch +
	                                  x * context.pixel_width);
	uint8_t *base_pixel = pixel;

	for (uint32_t row = 0; row < height; row++) {
		for (uint32_t column = 0; column < width; column++) {
			pixel[0] = color.b;
			pixel[1] = color.g;
			pixel[2] = color.r;

			pixel += context.pixel_width;
		}

		pixel = base_pixel;
		pixel += (row + 1) * context.pitch;
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
	context.framebuffer_size = (context.height * context.pitch) +
		(context.width * context.pixel_width);

	/* draw the background */
	graphics_rect(0, 0, context.width, context.height, background);
}