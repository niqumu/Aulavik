/*====------------ terminal.c - Framebuffer terminal emulator ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/terminal.h>

#include <string.h>

struct render_context *render_context;

struct ansi_state ansi_state;
struct terminal_state terminal_state;

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

/* process ("run") the current escape sequence */
static void terminal_process_escape()
{
	if (ansi_state.sequence == CONTROL_SEQUENCE) {

		/* todo: this is lacking a LOT of functionality */

		/* foreground provided */
		if (ansi_state.sequence_part_count == 1) {
			uint8_t fg_id = ansi_state.sequence_parts[0];
			terminal_state.color = get_ansi_color(fg_id);
		}

		/* foreground and background provided */
		else if (ansi_state.sequence_part_count == 2) {
			uint8_t fg_id = ansi_state.sequence_parts[0];
			terminal_state.color = get_ansi_color(fg_id);
//			uint8_t bg_id = ansi_state.sequence_parts[1];
//			terminal_state.color = get_ansi_color(fg_id);
		}
	}
}

/* build the current escape sequence with the next char */
static void terminal_parse_escape(char c)
{
	switch (c) {
	case CONTROL_SEQUENCE_INTRODUCER:
		ansi_state.sequence = CONTROL_SEQUENCE;
		ansi_state.sequence_part_count = 1;
		memset(ansi_state.sequence_parts, 0,
		       sizeof(ansi_state.sequence_parts));
		break;
	case SEPERATOR:
		ansi_state.sequence_part_count++;
		break;
	case ESCAPE_END:
		ansi_state.escaped = false;
		terminal_process_escape();
		break;
	default:
		if (c >= '0' && c <= '9') {
			int num = c - '0';
			ansi_state.sequence_parts[
				ansi_state.sequence_part_count - 1] *= 10;
			ansi_state.sequence_parts[
				ansi_state.sequence_part_count - 1] += num;
		}

		break;
	}
}

void terminal_putc(char c)
{
	if (ansi_state.escaped) {
		terminal_parse_escape(c);
		return;
	}

	switch (c) {
	case 0x08:
		terminal_state.x -= char_width + FR_KERNING;

		if (terminal_state.x < TERMINAL_PADDING)
			terminal_state.x = TERMINAL_PADDING;

		/* I have no idea where these values come from, but this
		 * perfectly covers up the old character */
		graphics_rect(terminal_state.x + 1, terminal_state.y + 9, char_width,
			      char_height, background);
		break;
	case '\n': /* line feed */
		terminal_state.x = TERMINAL_PADDING;
		terminal_state.y += char_height + FR_LINE_SPACING;
		break;
	case 0x1b: /* escape */
		ansi_state.escaped = true;
		break;
	default:
		fr_render_char(terminal_state.x, terminal_state.y,
			       c, terminal_state.color);
		terminal_state.x += char_width + FR_KERNING;
	}

	/* new line if x is too high */
	if (terminal_state.x + TERMINAL_PADDING > render_context->width) {
		terminal_state.x = TERMINAL_PADDING;
		terminal_state.y += char_height + FR_LINE_SPACING;
	}
}

void terminal_init(struct render_context *context)
{
	render_context = context;

	terminal_state.x = TERMINAL_PADDING;
	terminal_state.y = TERMINAL_PADDING;
	terminal_state.color = color_15;
}