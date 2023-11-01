#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *VGA_MEMORY = (uint16_t*) 0xb8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_put_at(char c, uint8_t color, size_t x, size_t y)
{
        const size_t index = (y * VGA_WIDTH) + x;
        terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(void)
{
        for (int i = VGA_WIDTH; i <= VGA_WIDTH * VGA_HEIGHT; i++) {
                VGA_MEMORY[i - VGA_WIDTH] = VGA_MEMORY[i];
        }

        for (int i = 0; i <= VGA_WIDTH; i++) {
                size_t row = VGA_HEIGHT - 1;
                VGA_MEMORY[(VGA_WIDTH * row) + i] = ' ';
        }
}

void terminal_putchar(char c)
{
        if (c == '\n') {
                terminal_column = 0;
                
                if (++terminal_row >= VGA_HEIGHT) {
                        terminal_scroll();
                        terminal_row = VGA_HEIGHT - 1;
                }

                return;
        }

        terminal_put_at(c, terminal_color, terminal_column, terminal_row);

        if (++terminal_column >= VGA_WIDTH) {
                terminal_column = 0;

                if (++terminal_row >= VGA_HEIGHT) {
                        terminal_scroll();
                        terminal_row = VGA_HEIGHT - 1;
                }
        }
}

void terminal_puts(const char *string)
{
        for (size_t c = 0; c < strlen(string); c++)
                terminal_putchar(string[c]);
}

void terminal_clear(void)
{
        for (size_t y = 0; y < VGA_HEIGHT; y++) {
                for (size_t x = 0; x < VGA_WIDTH; x++) {
                        terminal_put_at(' ', terminal_color, x, y);
                }
        }
}

void terminal_initialize(void)
{
        terminal_row = 0;
        terminal_column = 0;
        terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
        terminal_buffer = VGA_MEMORY;

        terminal_clear();
}

void terminal_setcolor(uint8_t color)
{
        terminal_color = color;
}
