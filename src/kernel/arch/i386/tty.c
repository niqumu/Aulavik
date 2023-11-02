/*====-------------- tty.c - VGA Teletype implementation -----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdint.h> /* uint16_t, uint8_t */

#include <kernel/tty.h>

#include "vga.h"

static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;
volatile uint16_t *VGA_MEMORY = (uint16_t*) 0xb8000;

int term_row;
int term_column;
uint8_t term_color;

static void terminal_put_at(char c, uint8_t color, int x, int y)
{
        const int index = (y * VGA_WIDTH) + x;
        VGA_MEMORY[index] = vga_entry(c, color);
}

static void terminal_scroll(void)
{
        for (int i = VGA_WIDTH; i <= VGA_WIDTH * VGA_HEIGHT; i++) {
                VGA_MEMORY[i - VGA_WIDTH] = VGA_MEMORY[i];
        }

        for (int i = 0; i <= VGA_WIDTH; i++) {
                int row = VGA_HEIGHT - 1;
                VGA_MEMORY[(VGA_WIDTH * row) + i] = ' ';
        }
}

static void terminal_clear(void)
{
        for (int y = 0; y < VGA_HEIGHT; y++) {
                for (int x = 0; x < VGA_WIDTH; x++) {
                        terminal_put_at(' ', term_color, x, y);
                }
        }
}

void terminal_putchar(char c)
{
        switch (c) {
        case 0x08: /* backspace */
                if (term_column > 0)
                        term_column--;
                break;
        case 0x09: /* tab */
                term_column = ((term_column + 7) / 8) * 8;
                break;
        case '\n': /* line feed */
                term_column = 0;
                term_row++;
                break;
        case 0x0d: /* carriage return */
                term_column = 0;
                break;
        default:
                terminal_put_at(c, term_color, term_column, term_row);
                term_column++;
        }

        if (term_column >= VGA_WIDTH) {
                term_column = 0;
                term_row++;
        }

        if (term_row >= VGA_HEIGHT) {
                terminal_scroll();
                term_row = VGA_HEIGHT - 1;
        }
}

void terminal_puts(const char *string)
{
        for (int i = 0; string[i]; i++) {
                char c = string[i];

                if (c == 0x1b) { /* escape */
                        // TODO: ANSI colors
                }

                terminal_putchar(c);
        }
}

void terminal_set_color(int fg, int bg)
{
        term_color = vga_entry_color(fg, bg);
}

void terminal_init(void)
{
        term_row = 0;
        term_column = 0;
        terminal_set_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
        terminal_clear();
}