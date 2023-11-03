/*====-------------- tty.h - VGA Teletype implementation -----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#define FE_CONTROL_SEQUENCE_INTRODUCER '['
#define FE_CONTROL_SEQUENCE FE_CONTROL_SEQUENCE_INTRODUCER

#define ESCAPE_START '\e'
#define ESCAPE_END 'm'
#define SEPERATOR ';'

/* initialize the terminal, clearing the screen */
void terminal_init(void);

/* write c in the terminal at the current position */
void terminal_putchar(char c);

/* write string in the terminal at the current position */
void terminal_puts(const char *string);

/* set the terminal color */
void terminal_set_color(int fg, int bg);

#endif /* _KERNEL_TTY_H */