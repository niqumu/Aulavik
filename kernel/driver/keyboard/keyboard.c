/*====---------------- keyboard.c - PS/2 keyboard driver -----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <kernel/driver/keyboard.h>

#include <kernel/driver/ports.h>
#include <kernel/idt/idt.h>
#include <kernel/logger.h>
#include <kernel/terminal.h>

int shift_down = 0;

void keyboard_handle_press(void)
{
	char resolved_char;
	uint8_t key = port_inb(PORT_PS2_RW_DATA);

	if (key == KEYCODE_LSHIFT_DOWN || key == KEYCODE_RSHIFT_DOWN) {
		shift_down = 1;
	} else if (key == KEYCODE_LSHIFT_UP || key == KEYCODE_RSHIFT_UP) {
		shift_down = 0;
	}

	if (shift_down)
		resolved_char = shift_scan_codes[key];
	else
		resolved_char = primary_scan_codes[key];

	if (resolved_char) {
		terminal_putc(resolved_char);
	}

	port_pic_eoi();
}

void keyboard_init(void)
{
	/* unmask keyboard irqs on the pic */
	idt_set_pic_mask(1, 0);

//	port_outb(PORT_PS2_RW_DATA, PS2_DEV_COMMAND_IDENTIFY);
//	k_debug("%x", port_inb(PORT_PS2_RW_DATA));

	k_ok("Loaded keyboard driver");
}