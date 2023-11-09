/*====------------- graphics.c - Tiny graphics library header ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
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

/* draw a pixel at the specified location, in the specified color. for bulk
 * rendering (e.g. rectangles), the specialized functions are far faster */
void graphics_plot_pixel(uint32_t x, uint32_t y, struct color color);

/* draw a rectangle of the specified dimensions at the specified location,
 * in the specified color */
void graphics_rect(uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height, struct color color);

void graphics_init(void);

#endif /* DRIVER_GRAPHICS_H */