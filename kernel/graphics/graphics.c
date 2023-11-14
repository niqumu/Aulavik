/*====---------------- graphics.c - Tiny graphics library ----------------====*\
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

#include <kernel/graphics/graphics.h>

#include <kernel/kernel.h>

struct render_context context;

struct render_context* graphics_get_context(void)
{
	return &context;
}

struct color graphics_color_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	struct color color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

struct color graphics_color(uint32_t rgb)
{
	struct color color;
	color.r = (rgb >> 16) & 0xff;
	color.g = (rgb >> 8) & 0xff;
	color.b = (rgb >> 0) & 0xff;
	return color;
}

/**
 * Draw a pixel at the specified location, in the specified color. For bulk
 * rendering, such as drawing a rectangle or other polygon, the specialized
 * functions are, by design, far faster.
 * @param x Pixel x coordinate
 * @param y Pixel y coordinate
 * @param color Pixel color
 */
void graphics_plot_pixel(uint32_t x, uint32_t y, struct color color)
{
	uint8_t *pixel = context.framebuffer + (y * context.pitch +
		x * context.pixel_width);

	pixel[0] = color.b;
	pixel[1] = color.g;
	pixel[2] = color.r;
}

/**
 * Draw a rectangle of the specified dimensions at the specified location,
 * in the specified color.
 * @param x Top-left corner x coordinate
 * @param y Top-left corner y coordinate
 * @param width Rectangle width
 * @param height Rectangle height
 * @param color Rectangle color
 */
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

void graphics_vgradient(uint32_t x, uint32_t y, uint32_t width,
                        uint32_t height, struct color color_top,
			struct color color_bottom)
{
	uint8_t *pixel = context.framebuffer + (y * context.pitch +
	                                        x * context.pixel_width);
	uint8_t *base_pixel = pixel;

	int delta_r = color_bottom.r - color_top.r;
	int delta_g = color_bottom.g - color_top.g;
	int delta_b = color_bottom.b - color_top.b;

	for (uint32_t row = 0; row < height; row++) {

		/* only calculate the color once per row */
		double p = ((double) row / (double) height);
		uint8_t b = (uint8_t) (color_top.b + (delta_b * p));
		uint8_t g = (uint8_t) (color_top.g + (delta_g * p));
		uint8_t r = (uint8_t) (color_top.r + (delta_r * p));

		for (uint32_t column = 0; column < width; column++) {
			pixel[0] = b;
			pixel[1] = g;
			pixel[2] = r;
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


}