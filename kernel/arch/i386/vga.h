/*====------------------- vga.h - VGA helper header ----------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h> /* uint8_t */

enum vga_shade {
        VGA_COLOR_BLACK = 0,
        VGA_COLOR_BLUE = 1,
        VGA_COLOR_GREEN = 2,
        VGA_COLOR_CYAN = 3,
        VGA_COLOR_RED = 4,
        VGA_COLOR_MAGENTA = 5,
        VGA_COLOR_BROWN = 6,
        VGA_COLOR_LIGHT_GRAY = 7,
        VGA_COLOR_DARK_GRAY = 8,
        VGA_COLOR_LIGHT_BLUE = 9,
        VGA_COLOR_LIGHT_GREEN = 10,
        VGA_COLOR_LIGHT_CYAN = 11,
        VGA_COLOR_LIGHT_RED = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN = 14,
        VGA_COLOR_WHITE = 15,
};

/* generate a vga memory entry from a character and style byte */
uint16_t vga_entry(unsigned char uc, uint8_t style)
{
        return (uint16_t) uc | (uint16_t) style << 8;
}

/* generate a vga color from foreground and background colors */
uint8_t vga_create_style(uint8_t fg, uint8_t bg)
{
        return fg | bg << 4;
}

/* get the vga color corresponding to the given id */
uint8_t vga_get_color(int id, int bg)
{
	if (bg)
		id -= 10;

	switch (id) {
	case 30: return VGA_COLOR_BLACK;
	case 31: return VGA_COLOR_RED;
	case 32: return VGA_COLOR_GREEN;
	case 33: return VGA_COLOR_BROWN;
	case 34: return VGA_COLOR_BLUE;
	case 35: return VGA_COLOR_MAGENTA;
	case 36: return VGA_COLOR_CYAN;
	case 37: return VGA_COLOR_LIGHT_GRAY;
	case 90: return VGA_COLOR_DARK_GRAY;
	case 91: return VGA_COLOR_LIGHT_RED;
	case 92: return VGA_COLOR_LIGHT_GREEN;
	case 93: return VGA_COLOR_LIGHT_BROWN;
	case 94: return VGA_COLOR_LIGHT_BLUE;
	case 95: return VGA_COLOR_LIGHT_MAGENTA;
	case 96: return VGA_COLOR_LIGHT_CYAN;
	case 97: return VGA_COLOR_WHITE;
	default: /* fg falls back to light gray, bg to black (defaults) */
		if (bg)
			return VGA_COLOR_BLACK;
		else
			return VGA_COLOR_LIGHT_GRAY;
	}
}

#endif /* ARCH_I386_VGA_H */