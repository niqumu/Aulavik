/*====------------------- FILENAME - SHORT DESCRIPTION -------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h>

#include <kernel/logger.h>
#include <kernel/kernel.h>

void graphics_init(void)
{
	multiboot_info_t *mb_info = kernel_get_mb_info();

	uint32_t bpp = mb_info->framebuffer_bpp;
	uint32_t pixel_width = bpp / 8;
	uint32_t pitch = mb_info->framebuffer_pitch;
	uint32_t width = mb_info->framebuffer_width;
	uint32_t height = mb_info->framebuffer_height;

	k_print("pixel_width: %d, pitch: %d, width: %d, height: %d", pixel_width, pitch, width, height);

	uint32_t size = mb_info->framebuffer_width * mb_info->framebuffer_height;
	uint8_t *vram = (uint8_t *) mb_info->framebuffer_addr;

	for (uint32_t x = 0; x <= width; x++) {
		for (uint32_t y = 0; y <= height; y++) {
			uint8_t *pixel = vram + y * pitch + x * pixel_width;

			pixel[0] = x / 3;
			pixel[1] = y / 2;
			pixel[2] = 255;
		}
	}
}