/*====------------ terminal.h - Framebuffer terminal emulator ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef GRAPHICS_TERMINAL_H
#define GRAPHICS_TERMINAL_H

#include <stdbool.h>

#include <kernel/graphics/font_renderer.h>
#include <kernel/graphics/graphics.h>

#define TERMINAL_PADDING        25
#define TAB                     8

#define CONTROL_SEQUENCE_INTRODUCER     '['
#define SEPERATOR                       ';'
#define ESCAPE_END                      'm'

enum ansi_escape_sequence {
	CONTROL_SEQUENCE,
	NONE
};

struct ansi_state {
	bool escaped;
	enum ansi_escape_sequence sequence;
	uint8_t sequence_part_count;
	uint8_t sequence_parts[10];
};

struct terminal_state {
	struct color foreground;
	struct color background;
	uint16_t x, y;
	uint8_t ticks;
	uint8_t cursor_type_ticks;
};

char* terminal_get_input(void);

void terminal_handle_key(char c);

void terminal_putc(char c);

void terminal_puts(const char *s);

void terminal_tick(void);

void terminal_clear(void);

void terminal_exit(void);

void terminal_init(struct render_context *context);

/* converts an ANSI color code to a terminal color */
static struct color get_ansi_color(uint8_t color)
{
	switch (color) {
		case 30: return color_0;
		case 31: return color_1;
		case 32: return color_2;
		case 33: return color_3;
		case 34: return color_4;
		case 35: return color_5;
		case 36: return color_6;
		case 37: return color_7;
		case 90: return color_8;
		case 91: return color_9;
		case 92: return color_10;
		case 93: return color_11;
		case 94: return color_12;
		case 95: return color_13;
		case 96: return color_14;
		case 97:
		default: return color_15;
	}
}

#endif /* GRAPHICS_TERMINAL_H */