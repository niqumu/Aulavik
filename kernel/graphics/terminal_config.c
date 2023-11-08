/*====--------------- terminal_config.c - Terminal config ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/graphics/graphics.h>

#define THEME 1

struct color background = {
#if (THEME == 1)
	0x10, 0x10, 0x10
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_0 = {
#if (THEME == 1)
	0x20, 0x20, 0x20
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_1 = {
#if (THEME == 1)
	0xb9, 0x1e, 0x2e
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_2 = {
#if (THEME == 1)
	0x81, 0x95, 0x7c
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_3 = {
#if (THEME == 1)
	0xf9, 0xbb, 0x80
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_4 = {
#if (THEME == 1)
	0x35, 0x65, 0x79
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_5 = {
#if (THEME == 1)
	0x2d, 0x20, 0x31
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_6 = {
#if (THEME == 1)
	0x0b, 0x34, 0x52
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_7 = {
#if (THEME == 1)
	0x90, 0x90, 0x90
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_8 = {
#if (THEME == 1)
	0x60, 0x60, 0x60
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_9 = {
#if (THEME == 1)
	0xd1, 0x45, 0x48
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_10 = {
#if (THEME == 1)
	0xa7, 0xb7, 0x9a
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_11 = {
#if (THEME == 1)
	0xfa, 0xe3, 0xa0
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_12 = {
#if (THEME == 1)
	0x74, 0x91, 0xa1
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_13 = {
#if (THEME == 1)
	0x87, 0x31, 0x4e
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_14 = {
#if (THEME == 1)
	0x0f, 0x82, 0x9d
#else
	0x1d, 0x1f, 0x21
#endif
};

struct color color_15 = {
#if (THEME == 1)
	0xff, 0xf0, 0xf0
#else
	0x1d, 0x1f, 0x21
#endif
};