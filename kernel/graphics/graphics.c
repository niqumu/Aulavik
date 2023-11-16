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

struct render_context global_rctx;

struct render_context* graphics_get_global_rctx(void)
{
	return &global_rctx;
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

void graphics_bake_contexts(struct render_context src, int src_x, int src_y,
                            int dest_x, int dest_y, int width, int height,
                            struct render_context dest)
{
	uint8_t *dest_pixel = dest.framebuffer + (dest_y * dest.pitch +
	                                     dest_x * dest.pixel_width);
	uint8_t *src_pixel = src.framebuffer + (src_y * src.pitch +
	                                         src_x * src.pixel_width);
	uint8_t *dest_base_pixel = dest_pixel;
	uint8_t *src_base_pixel = src_pixel;

	bool culling_needed = dest_x < 0 || dest_y < 0 ||
	        dest_x + width > (int) dest.width ||
		dest_y + height > (int) dest.height;

	for (int row = 0; row < height; row++) {
		for (int column = 0; column < width; column++) {

			if (culling_needed && dest_x + column >= (int) dest.width)
				goto end_row;

			if (culling_needed && (dest_x + column < 0 ||
					dest_y + row < 0))
				goto end_column;

			dest_pixel[0] = src_pixel[0];
			dest_pixel[1] = src_pixel[1];
			dest_pixel[2] = src_pixel[2];

end_column:
			src_pixel += src.pixel_width;
			dest_pixel += dest.pixel_width;
		}

end_row:
		src_pixel = src_base_pixel;
		src_pixel += (row + 1) * src.pitch;
		dest_pixel = dest_base_pixel;
		dest_pixel += (row + 1) * dest.pitch;

		if (culling_needed && dest_y + row >= (int) dest.height)
			return;
	}
}

/**
 * Draw a pixel at the specified location, in the specified color. For bulk
 * rendering, such as drawing a rectangle or other polygon, the specialized
 * functions are, by design, far faster.
 * @param ctx The render context to draw to
 * @param x Pixel x coordinate
 * @param y Pixel y coordinate
 * @param color Pixel color
 */
void graphics_plot_pixel(struct render_context ctx, uint32_t x, uint32_t y,
			 struct color color)
{
	uint8_t *pixel = ctx.framebuffer + (y * ctx.pitch +
		x * ctx.pixel_width);

	pixel[0] = color.b;
	pixel[1] = color.g;
	pixel[2] = color.r;
}

/**
 * Draw a rectangle of the specified dimensions at the specified location,
 * in the specified color.
 * @param ctx The render context to draw to
 * @param x Top-left corner x coordinate
 * @param y Top-left corner y coordinate
 * @param width Rectangle width
 * @param height Rectangle height
 * @param color Rectangle color
 */
void graphics_rect(struct render_context ctx, uint32_t x, uint32_t y,
		   uint32_t width, uint32_t height, struct color color)
{
	uint8_t *pixel = ctx.framebuffer + (y * ctx.pitch +
	                                  x * ctx.pixel_width);
	uint8_t *base_pixel = pixel;
	bool culling_needed = x + width > ctx.width || y + height > ctx.height;

	for (uint32_t row = 0; row < height; row++) {
		for (uint32_t column = 0; column < width; column++) {
			pixel[0] = color.b;
			pixel[1] = color.g;
			pixel[2] = color.r;

			pixel += ctx.pixel_width;

			if (culling_needed && x + column >= ctx.width)
				goto end_row;
		}

end_row:
		pixel = base_pixel;
		pixel += (row + 1) * ctx.pitch;

		if (culling_needed && y + row >= ctx.height)
			return;
	}
}

void graphics_vgradient(struct render_context ctx, uint32_t x, uint32_t y,
			uint32_t width, uint32_t height, struct color color_top,
			struct color color_bottom)
{
	uint8_t *pixel = ctx.framebuffer + (y * ctx.pitch + x * ctx.pixel_width);
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
			pixel += ctx.pixel_width;
		}

		pixel = base_pixel;
		pixel += (row + 1) * ctx.pitch;
	}
}

void graphics_hgradient(struct render_context ctx, uint32_t x, uint32_t y,
                        uint32_t width, uint32_t height, struct color color_left,
                        struct color color_right)
{
	uint8_t *pixel = ctx.framebuffer + (y * ctx.pitch + x * ctx.pixel_width);
	uint8_t *base_pixel = pixel;

	int delta_r = color_left.r - color_right.r;
	int delta_g = color_left.g - color_right.g;
	int delta_b = color_left.b - color_right.b;

	for (uint32_t column = 0; column < width; column++) {

		/* only calculate the color once per column */
		double p = ((double) column / (double) width);
		uint8_t b = (uint8_t) (color_left.b + (delta_b * p));
		uint8_t g = (uint8_t) (color_left.g + (delta_g * p));
		uint8_t r = (uint8_t) (color_left.r + (delta_r * p));

		for (uint32_t row = 0; row < height; row++) {
			pixel[0] = b;
			pixel[1] = g;
			pixel[2] = r;
			pixel += ctx.pitch;
		}

		pixel = base_pixel;
		pixel += (column + 1) * ctx.pixel_width;
	}
}


void graphics_init(void)
{
	global_rctx.framebuffer = (uint8_t *) mb_info->framebuffer_addr;
	global_rctx.width = mb_info->framebuffer_width;
	global_rctx.height = mb_info->framebuffer_height;
	global_rctx.pixel_width = mb_info->framebuffer_bpp / 8;
	global_rctx.pitch = mb_info->framebuffer_pitch;
	global_rctx.framebuffer_size = (global_rctx.height * global_rctx.pitch) +
	                                (global_rctx.width * global_rctx.pixel_width);
}