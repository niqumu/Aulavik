/*====-------------- tty.c - VGA Teletype implementation -----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h> /* uint16_t, uint8_t */
#include <string.h> /* memset() */

#include <kernel/driver/ports.h>
#include <kernel/driver/tty.h>
#include <kernel/logger.h>

#include "vga.h"

static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;
volatile uint16_t *VGA_MEMORY = (uint16_t*) 0xb8000;

/* cache of the text mode cursor state */
int term_cursor_enabled;
int term_cursor_row;
int term_cursor_column;

/* cache of the terminal style state */
uint8_t term_style;
uint8_t term_fg_color;
uint8_t term_bg_color;

/* ANSI escape sequence state */
int escaped = 0;
char sequence;
int ctrl_seq_part_count = 0;
int ctrl_seq_parts[10] = {0};

void terminal_set_color(int fg, int bg)
{
	term_fg_color = fg;
	term_bg_color = bg;
	term_style = vga_create_style(fg, bg);
}

int terminal_get_color(int *fg_dest, int *bg_dest)
{
	*fg_dest = term_fg_color;
	*bg_dest = term_bg_color;
	return term_style;
}

void terminal_set_cursor(int row, int col)
{
	uint16_t position = col + (row * VGA_WIDTH);
	port_outb(0x3d4, 0x0f);
	port_outb(0x3d5, (uint8_t) (position & 0xff));
	port_outb(0x3d4, 0x0e);
	port_outb(0x3d5, (uint8_t) ((position >> 8) & 0xff));

	term_cursor_row = row;
	term_cursor_column = col;
}

void terminal_set_cursor_enabled(int state)
{
	if ((term_cursor_enabled = state)) {
		port_outb(0x3d4, 0x0a);
		port_outb(0x3d5, (port_inb(0x3d5) & 0xc0) | 0);
		port_outb(0x3d4, 0x0b);
		port_outb(0x3d5, (port_inb(0x3d5) & 0xc0) | VGA_HEIGHT);
	} else {
		port_outb(0x3d4, 0x0a);
		port_outb(0x3d5, 0x20);
	}
}

static void terminal_put_at(char c, uint8_t color, int x, int y)
{
	const int index = (y * VGA_WIDTH) + x;
	VGA_MEMORY[index] = vga_entry(c, color);
}

static void terminal_scroll(void)
{
	for (int i = VGA_WIDTH; i <= VGA_WIDTH * VGA_HEIGHT; i++)
		VGA_MEMORY[i - VGA_WIDTH] = VGA_MEMORY[i];

	for (int i = 0; i <= VGA_WIDTH; i++) {
		int row = VGA_HEIGHT - 1;
		VGA_MEMORY[(VGA_WIDTH * row) + i] = ' ';
	}
}

static void terminal_clear(void)
{
	for (int y = 0; y < VGA_HEIGHT; y++) {
		for (int x = 0; x < VGA_WIDTH; x++)
			terminal_put_at(' ', term_style, x, y);
	}
}

/* process ("run") the current escape sequence */
static void terminal_process_escape()
{
	if (sequence == FE_CONTROL_SEQUENCE) {

		/* todo: this is lacking a LOT of functionality */

		/* foreground provided */
		if (ctrl_seq_part_count == 1) {
			uint8_t fg_id = ctrl_seq_parts[0];
			uint8_t fg = vga_get_color(fg_id, 0);
			terminal_set_color(fg, term_bg_color);
		}

		/* foreground and background provided */
		else if (ctrl_seq_part_count == 2) {
			uint8_t fg_id = ctrl_seq_parts[0];
			uint8_t fg = vga_get_color(fg_id, 0);
			uint8_t bg_id = ctrl_seq_parts[1];
			uint8_t bg = vga_get_color(bg_id, 1);
			terminal_set_color(fg, bg);
		}
	}
}

/* build the current escape sequence with the next char */
static void terminal_parse_escape(char next)
{
	switch (next) {
	case FE_CONTROL_SEQUENCE_INTRODUCER:
		sequence = FE_CONTROL_SEQUENCE;
		ctrl_seq_part_count = 1; /* having 0 parts is impossible */
		memset(ctrl_seq_parts, 0, sizeof(ctrl_seq_parts));
		break;
	case SEPERATOR:
		ctrl_seq_part_count++;
		break;
	case ESCAPE_END:
		escaped = 0;
		terminal_process_escape();
		break;
	default:
		if (next >= '0' && next <= '9') {
			int num = next - '0';
			ctrl_seq_parts[ctrl_seq_part_count - 1] *= 10;
			ctrl_seq_parts[ctrl_seq_part_count - 1] += num;
		}

		break;
	}
}

void terminal_putchar(char c)
{
	if (escaped) {
		terminal_parse_escape(c);
		return;
	}

	switch (c) {
	case 0x08: /* backspace */
		if (term_cursor_column > 0) {
			terminal_set_cursor(term_cursor_row, term_cursor_column - 1);
		}
		terminal_put_at(' ', term_style, term_cursor_column, term_cursor_row);
		break;
	case 0x09: /* tab */
		int column = ((term_cursor_column + 7 + 1) / 8) * 8;
		terminal_set_cursor(term_cursor_row , column);
		break;
	case '\n': /* line feed */
		terminal_set_cursor(term_cursor_row + 1, 0);
		break;
	case 0x0d: /* carriage return */
		terminal_set_cursor(term_cursor_row, 0);
		break;
	case 0x1b: /* escape */
		escaped = 1;
		break;
	default:
		terminal_put_at(c, term_style, term_cursor_column, term_cursor_row);
		terminal_set_cursor(term_cursor_row, term_cursor_column + 1);
	}

	if (term_cursor_column >= VGA_WIDTH) {
		terminal_set_cursor(term_cursor_row + 1, 0);
	}

	if (term_cursor_row >= VGA_HEIGHT) {
		terminal_scroll();
		terminal_set_cursor(VGA_HEIGHT - 1, 0);
	}
}

void terminal_puts(const char *string)
{
	for (int i = 0; string[i]; i++)
		terminal_putchar(string[i]);
}

void terminal_init(void)
{
	terminal_set_cursor(0, 0);
	terminal_set_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
	terminal_clear();

	k_ok("Initialized terminal");
}