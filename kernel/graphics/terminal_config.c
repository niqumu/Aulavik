/*====--------------- terminal_config.c - Terminal config ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/graphics/graphics.h>

/*
 * 1: vacuous2 by hal
 * 2: mashup colors by user 77
 */
#define THEME 1

struct color background = {
#if (THEME == 1)
	0x10, 0x10, 0x10
#elif (THEME == 2)
	0x17, 0x17, 0x17
#endif
};

struct color color_0 = {
#if (THEME == 1)
	0x20, 0x20, 0x20
#elif (THEME == 2)
	0x20, 0x2, 0x20
#endif
};

struct color color_1 = {
#if (THEME == 1)
	0xb9, 0x1e, 0x2e
#elif (THEME == 2)
	0xbf, 0x3f, 0x34
#endif
};

struct color color_2 = {
#if (THEME == 1)
	0x81, 0x95, 0x7c
#elif (THEME == 2)
	0x70, 0xd2, 0x22
#endif
};

struct color color_3 = {
#if (THEME == 1)
	0xf9, 0xbb, 0x80
#elif (THEME == 2)
	0xbf, 0x7a, 0x29
#endif
};

struct color color_4 = {
#if (THEME == 1)
	0x35, 0x65, 0x79
#elif (THEME == 2)
	0x62, 0x7a, 0x92
#endif
};

struct color color_5 = {
#if (THEME == 1)
	0x2d, 0x20, 0x31
#elif (THEME == 2)
	0x75, 0x50, 0x7b
#endif
};

struct color color_6 = {
#if (THEME == 1)
	0x0b, 0x34, 0x52
#elif (THEME == 2)
	0x75, 0x79, 0x78
#endif
};

struct color color_7 = {
#if (THEME == 1)
	0x90, 0x90, 0x90
#elif (THEME == 2)
	0xb2, 0xa1, 0x91
#endif
};

struct color color_8 = {
#if (THEME == 1)
	0x60, 0x60, 0x60
#elif (THEME == 2)
	0x40, 0x40, 0x40
#endif
};

struct color color_9 = {
#if (THEME == 1)
	0xd1, 0x45, 0x48
#elif (THEME == 2)
	0xff, 0x6c, 0x5f
#endif
};

struct color color_10 = {
#if (THEME == 1)
	0xa7, 0xb7, 0x9a
#elif (THEME == 2)
	0xb8, 0xca, 0x4b
#endif
};

struct color color_11 = {
#if (THEME == 1)
	0xfa, 0xe3, 0xa0
#elif (THEME == 2)
	0xc7, 0xa5, 0x5a
#endif
};

struct color color_12 = {
#if (THEME == 1)
	0x74, 0x91, 0xa1
#elif (THEME == 2)
	0x95, 0xb9, 0xde
#endif
};

struct color color_13 = {
#if (THEME == 1)
	0x87, 0x31, 0x4e
#elif (THEME == 2)
	0xad, 0x7f, 0xa8
#endif
};

struct color color_14 = {
#if (THEME == 1)
	0x0f, 0x82, 0x9d
#elif (THEME == 2)
	0x9f, 0xa5, 0x90
#endif
};

struct color color_15 = {
#if (THEME == 1)
	0xff, 0xf0, 0xf0
#elif (THEME == 2)
	0xe8, 0xd0, 0xc3
#endif
};