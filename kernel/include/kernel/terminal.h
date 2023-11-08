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

#define TERMINAL_PADDING        15

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
	struct color color;
	uint16_t x, y;
};

void terminal_putc(char c);

void terminal_init(struct render_context *context);

#endif /* GRAPHICS_TERMINAL_H */