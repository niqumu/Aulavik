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
#include <string.h>

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

/**
 * Bakes the contents of the framebuffer of src onto the framebuffer of dest.
 * The contents are read from the source starting at (src_x, src_y), up to the
 * provided width and height, and drawn onto the detination framebuffer
 * starting at (dest_x, dest_y).
 *
 * If the source has blending enabled, any pure black pixels in the source
 * framebuffer will be skipped, retaining the original pixel of the destination
 * framebuffer.
 *
 * @param src The render context to be baked onto the destination framebuffer
 * @param src_x The x-position on the source framebuffer to begin reading from;
 *      the x-coordinate of the top-left corner of the region to bake/paste
 * @param src_y The y-position on the source framebuffer to begin reading from;
 *      the y-coordinate of the top-left corner of the region to bake/paste
 * @param dest_x The x-position of the top-left corner on the destination
 *      framebuffer to paste to
 * @param dest_y The y-position of the top-left corner on the destination
 *      framebuffer to paste to
 * @param width The width of the region to copy
 * @param height The height of the region to copy
 * @param dest The render context containing the destination framebuffer
 */
void graphics_bake_contexts(struct render_context *src, int src_x, int src_y,
                            int dest_x, int dest_y, uint32_t width,
			    uint32_t height, struct render_context *dest)
{
	/*
	 * if we are doing a simple copy of an entire buffer onto another
	 * identical buffer without moving or clipping, we can do a fast
	 * copy using memcpy()
	 */
	if (src_x == 0 && src_y == 0 && dest_x == 0 && dest_y == 0 &&
			width == src->width && height == src->height &&
			width == dest->width && height == dest->height) {
		memcpy(dest->framebuffer, src->framebuffer, src->framebuffer_size);
		return;
	}

	if (dest_x < 0) {
		width -= (0 - dest_x);
		src_x += (0 - dest_x);
		dest_x = 0;
	}

	if (dest_y < 0) {
		height -= (0 - dest_y);
		src_y += (0 - dest_y);
		dest_y = 0;
	}

	if (dest_x + width > dest->width) {
		width -= ((dest_x + width) - dest->width);
	}

	if (dest_y + height > dest->height) {
		height -= ((dest_y + height) - dest->height);
	}

	register uint8_t *dest_pixel = dest->framebuffer + (dest_y * dest->pitch +
		dest_x * dest->pixel_width);
	register uint8_t *src_pixel = src->framebuffer + (src_y * src->pitch +
		src_x * src->pixel_width);
	uint8_t *dest_base_pixel = dest_pixel;
	uint8_t *src_base_pixel = src_pixel;

	for (uint32_t row = 0; row < height; row++) {
		for (uint32_t column = 0; column < width; column++) {

			/*
			 * skip drawing the pixel if the source framebuffer has
			 * blending enabled and this pixel is solid black,
			 * preserving the original value the destination
			 * framebuffer has at this coordinate.
			 */
			if (src->blending && !src_pixel[0] &&
					!src_pixel[1] && !src_pixel[2]) {
				goto end_column;
			}

			dest_pixel[0] = src_pixel[0];
			dest_pixel[1] = src_pixel[1];
			dest_pixel[2] = src_pixel[2];

end_column:
			src_pixel += src->pixel_width;
			dest_pixel += dest->pixel_width;
		}

		src_pixel = src_base_pixel;
		src_pixel += (row + 1) * src->pitch;
		dest_pixel = dest_base_pixel;
		dest_pixel += (row + 1) * dest->pitch;
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
void graphics_plot_pixel(struct render_context *ctx, uint32_t x, uint32_t y,
			 struct color color)
{
	uint8_t *pixel = ctx->framebuffer + (y * ctx->pitch +
		x * ctx->pixel_width);

	pixel[0] = color.b;
	pixel[1] = color.g;
	pixel[2] = color.r;
}

/**
 * Draw a rectangle of the specified dimensions at the specified location,
 * in the specified color, onto ctx's framebuffer.
 * @param ctx The render context to draw to
 * @param x Top-left corner x coordinate
 * @param y Top-left corner y coordinate
 * @param width Rectangle width
 * @param height Rectangle height
 * @param color Rectangle color
 */
void graphics_rect(struct render_context *ctx, uint32_t x, uint32_t y,
                   uint32_t width, uint32_t height, struct color color)
{
	register uint8_t *pixel = ctx->framebuffer + (y * ctx->pitch +
	                                              x * ctx->pixel_width);
	uint8_t *base_pixel = pixel;
	bool culling_needed = x + width > ctx->width || y + height > ctx->height;

	for (uint32_t row = 0; row < height; row++) {
		for (uint32_t column = 0; column < width; column++) {
			pixel[0] = color.b;
			pixel[1] = color.g;
			pixel[2] = color.r;

			pixel += ctx->pixel_width;

			/* skip to the next row if we're off the x-axis */
			if (culling_needed && x + column >= ctx->width)
				goto end_row;
		}

end_row:
		pixel = base_pixel;
		pixel += (row + 1) * ctx->pitch;

		/* stop drawing if we're off the y-axis */
		if (culling_needed && y + row >= ctx->height)
			return;
	}
}

void graphics_outline(struct render_context *ctx, uint32_t x, uint32_t y,
                      uint32_t width, uint32_t height, uint8_t thickness,
                      struct color color)
{
	graphics_rect(ctx, x, y, width, thickness, color);
	graphics_rect(ctx, x, y + height - thickness, width, thickness, color);
	graphics_rect(ctx, x, y, thickness, height, color);
	graphics_rect(ctx, x + width - thickness, y, height, thickness, color);
}

void graphics_draw_mcr(struct render_context *ctx, const uint8_t *buffer,
                       uint32_t x, uint32_t y)
{
	uint32_t width = buffer[0];
	uint32_t height = buffer[1];
	struct color primary = {buffer[2], buffer[3], buffer[4]};
	struct color secondary = {buffer[5], buffer[6], buffer[7]};

	uint32_t bytes = (height * width) / 8;

	bool prim_trans = !primary.r && !primary.g && !primary.b;
	bool sec_trans = !secondary.r && !secondary.g && !secondary.b;

	/* rendering starts at the top left */
	uint32_t draw_x = x;
	uint32_t draw_y = y;

	/* iterate over each byte in the character */
	for (uint32_t i = 0; i < bytes; i++) {

		uint8_t byte = buffer[i + 8];

		/* iterate over each bit in the byte */
		for (uint8_t j = 8; j > 0; j--) {
			bool bit = byte & (1 << (j - 1));

			if (bit && !prim_trans)
				graphics_plot_pixel(ctx, draw_x, draw_y,primary);

			else if (!bit && !sec_trans)
				graphics_plot_pixel(ctx, draw_x, draw_y,secondary);

			if (++draw_x >= width) {
				draw_x = x;
				draw_y++;
			}
		}
	}

}

void graphics_vgradient(struct render_context *ctx, uint32_t x, uint32_t y,
			uint32_t width, uint32_t height, struct color color_top,
			struct color color_bottom)
{
	uint8_t *pixel = ctx->framebuffer + (y * ctx->pitch + x * ctx->pixel_width);
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
			pixel += ctx->pixel_width;
		}

		pixel = base_pixel;
		pixel += (row + 1) * ctx->pitch;
	}
}

void graphics_hgradient(struct render_context *ctx, uint32_t x, uint32_t y,
                        uint32_t width, uint32_t height, struct color color_left,
                        struct color color_right)
{
	uint8_t *pixel = ctx->framebuffer + (y * ctx->pitch + x * ctx->pixel_width);
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
			pixel += ctx->pitch;
		}

		pixel = base_pixel;
		pixel += (column + 1) * ctx->pixel_width;
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