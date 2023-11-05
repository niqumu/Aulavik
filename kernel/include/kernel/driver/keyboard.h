/*====------------- keyboard.h - PS/2 keyboard driver header -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _DRIVER_KEYBOARD_H
#define _DRIVER_KEYBOARD_H

/* outgoing commands to keyboard */
#define KBD_COMMAND_SET_LEDS                0xed
#define KBD_COMMAND_SCAN_CODE_SET           0xf0
#define KBD_COMMAND_TYPEMATIC_CFG           0xf3
#define KBD_COMMAND_USE_DEFAULTS            0xf6
#define KBD_COMMAND_TYPEMATIC_ONLY_ALL      0xf7
#define KBD_COMMAND_MAKE_REL_ALL            0xf8
#define KBD_COMMAND_MAKE_ONLY_ALL           0xf9
#define KBD_COMMAND_TYPEMATIC_ONE           0xfb
#define KBD_COMMAND_MAKE_REL_ONE            0bfc
#define KBD_COMMAND_MAKE_ONLY_ONE           0xfd

void keyboard_handle_press();

#endif /* _DRIVER_KEYBOARD_H */