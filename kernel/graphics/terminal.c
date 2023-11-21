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

#include <kernel/graphics/font.h>
#include <kernel/interrupts/pic.h>

struct render_context *render_context;

struct ansi_state ansi_state;
struct terminal_state terminal_state;

volatile char input[512];
uint16_t input_index;
volatile bool enter_pressed, dirty = false;

bool terminal_alive = false;

void terminal_render_cursor(bool state)
{
	struct color color = state ? color_7 : background;
	graphics_rect(graphics_get_global_rctx(), terminal_state.x + 1,
		      terminal_state.y + 1,font_width,font_height, color);
}

void terminal_tick(void)
{
	if (!terminal_alive)
		return;

	switch (terminal_state.ticks) {
	case 7:
		if (!terminal_state.cursor_type_ticks)
			terminal_render_cursor(false);

		break;
	case 14:
		terminal_render_cursor(true);
		terminal_state.ticks = 0;
		break;
	}

	terminal_state.ticks++;

	if (terminal_state.cursor_type_ticks)
		terminal_state.cursor_type_ticks--;
}

char* terminal_get_input(void)
{
	while (!enter_pressed)
		;

	dirty = true;
	enter_pressed = false;
	return (char *) &input[0];
}

void terminal_handle_key(char c)
{
	if (dirty || enter_pressed) {
		enter_pressed = false;
		dirty = false;
		input_index = 0;
		memset(&input[0], 0, 512);
	}

	switch (c) {
	case 0x08: /* backspace */
		if (!input_index)
			return;

		input_index--;
		input[input_index] = '\0';
		break;
	case '\n':
		enter_pressed = true;
		break;
	default:
		if (input_index >= 511)
			break;

		input[input_index] = c;
		input_index++;
	}

	terminal_putc(c);
}

/* process ("run") the current escape sequence */
static void terminal_process_escape()
{
	if (ansi_state.sequence == CONTROL_SEQUENCE) {

		/* todo: this is lacking a LOT of functionality */

		/* foreground provided */
		if (ansi_state.sequence_part_count == 1) {

			/* reset */
			if (!ansi_state.sequence_parts[0]) {
				terminal_state.foreground = color_15;
				terminal_state.background = background;
				return;
			}

			uint8_t fg = ansi_state.sequence_parts[0];
			terminal_state.foreground = get_ansi_color(fg);
		}

		/* foreground and background provided */
		else if (ansi_state.sequence_part_count == 2) {
			uint8_t fg = ansi_state.sequence_parts[0];
			terminal_state.foreground = get_ansi_color(fg);
			uint8_t bg = ansi_state.sequence_parts[1];
			terminal_state.background = get_ansi_color(bg - 10);
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
	if (!terminal_alive)
		return;

	/* remove the cursor */
	terminal_render_cursor(false);

	if (ansi_state.escaped) {
		terminal_parse_escape(c);
		return;
	}

	switch (c) {
	case 0x08: /* backspace */
		terminal_state.x -= font_width + FR_KERNING;

		if (terminal_state.x < TERMINAL_PADDING)
			terminal_state.x = TERMINAL_PADDING;

		terminal_render_cursor(false);
		break;
	case 0x09: /* tab */
		terminal_state.x = ((terminal_state.x + TAB) / TAB) * TAB;
		break;
	case '\n': /* line feed */
		terminal_state.x = TERMINAL_PADDING;
		terminal_state.y += font_height + FR_LINE_SPACING;
		break;
	case 0x1b: /* escape */
		ansi_state.escaped = true;
		break;
	default:
		/* character background color */
		graphics_rect(graphics_get_global_rctx(), terminal_state.x,
			      terminal_state.y - 1, font_width + 2,
			      font_height + 3, terminal_state.background);

		fr_render_char(*graphics_get_global_rctx(), terminal_state.x,
			       terminal_state.y, c, terminal_state.foreground);
		terminal_state.x += font_width + FR_KERNING;
	}

	/* new line if x is too high */
	if (terminal_state.x + TERMINAL_PADDING > render_context->width) {
		terminal_state.x = TERMINAL_PADDING;
		terminal_state.y += font_height + FR_LINE_SPACING;
	}

	/* scroll if y is too high */
	if (terminal_state.y + TERMINAL_PADDING > render_context->height) {
		/* todo this isn't easy and I can't think of a sane way */

		uint16_t line_size = font_height + FR_LINE_SPACING;
		uint16_t overscan = line_size * 10;
		terminal_state.y -= overscan;

		uint32_t i;
		for (i = 0; i < render_context->framebuffer_size; i++) {
			render_context->framebuffer[i] = render_context->
				framebuffer[i +
				(render_context->pitch * overscan)];
		}

		graphics_rect(graphics_get_global_rctx(), 0,
			      render_context->height - overscan,
			      render_context->width, overscan,
			      background);
	}

	/* redraw cursor at the new position */
	terminal_render_cursor(true);
	terminal_state.cursor_type_ticks = 3;
}

void terminal_puts(const char *str)
{
	if (!terminal_alive)
		return;

	for (int i = 0; str[i]; i++)
		terminal_putc(str[i]);
}

void terminal_clear(void)
{
	if (!terminal_alive)
		return;

	terminal_state.x = TERMINAL_PADDING;
	terminal_state.y = TERMINAL_PADDING;
	graphics_rect(graphics_get_global_rctx(), 0, 0, render_context->width,
		      render_context->height, background);
}

void terminal_exit(void)
{
	terminal_alive = false;
}

void terminal_init(struct render_context *context)
{
	render_context = context;

	graphics_rect(context, 0, 0, context->width,
		      context->height, background);

	terminal_state.x = TERMINAL_PADDING;
	terminal_state.y = TERMINAL_PADDING;
	terminal_state.foreground = color_15;
	terminal_state.background = background;

	terminal_alive = true;
}