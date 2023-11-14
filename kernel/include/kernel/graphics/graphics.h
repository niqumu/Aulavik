/*====------------- graphics.c - Tiny graphics library header ------------====*\
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

#ifndef DRIVER_GRAPHICS_H
#define DRIVER_GRAPHICS_H

#include <stdint.h>

struct color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct render_context {
	uint8_t *framebuffer;
	uint32_t framebuffer_size;      // the size of the framebuffer in bytes

	uint32_t width;                 // pixel width of the framebuffer
	uint32_t height;                // pixel height of the framebuffer

	uint32_t bpp;                   // bits per pixel
	uint32_t pixel_width;           // byte size of a pixel
	uint32_t pitch;                 // byte size of a row
};

extern struct color background;
extern struct color color_0;
extern struct color color_1;
extern struct color color_2;
extern struct color color_3;
extern struct color color_4;
extern struct color color_5;
extern struct color color_6;
extern struct color color_7;
extern struct color color_8;
extern struct color color_9;
extern struct color color_10;
extern struct color color_11;
extern struct color color_12;
extern struct color color_13;
extern struct color color_14;
extern struct color color_15;

struct render_context* graphics_get_context(void);

struct color graphics_color_rgb(uint8_t r, uint8_t g, uint8_t b);

struct color graphics_color(uint32_t rgb);

/**
 * Draw a pixel at the specified location, in the specified color. For bulk
 * rendering, such as drawing a rectangle or other polygon, the specialized
 * functions are, by design, far faster.
 * @param x Pixel x coordinate
 * @param y Pixel y coordinate
 * @param color Pixel color
 */
void graphics_plot_pixel(uint32_t x, uint32_t y, struct color color);

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
                   uint32_t height, struct color color);

void graphics_vgradient(uint32_t x, uint32_t y, uint32_t width,
                        uint32_t height, struct color color_top,
                        struct color color_bottom);

void graphics_init(void);

#endif /* DRIVER_GRAPHICS_H */