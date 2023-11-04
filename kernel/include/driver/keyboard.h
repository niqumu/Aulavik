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
#define COMMAND_SET_LEDS                0xed
#define COMMAND_ECHO                    0xee
#define COMMAND_SCAN_CODE_SET           0xf0
#define COMMAND_IDENTIFY                0xf2
#define COMMAND_TYPEMATIC_CFG           0xf3
#define COMMAND_ENABLE_SCAN             0xf4
#define COMMAND_DISABLE_SCAN            0xf5
#define COMMAND_USE_DEFAULTS            0xf6
#define COMMAND_TYPEMATIC_ONLY_ALL      0xf7
#define COMMAND_MAKE_REL_ALL            0xf8
#define COMMAND_MAKE_ONLY_ALL           0xf9
#define COMMAND_TYPEMATIC_ONE           0xfb
#define COMMAND_MAKE_REL_ONE            0bfc
#define COMMAND_MAKE_ONLY_ONE           0xfd
#define COMMAND_RESEND                  0xfe
#define COMMAND_SELF_TEST               0xff

/* incoming messages from keyboard */
#define MESSAGE_ERROR_1                 0x00
#define MESSAGE_SELF_TEST_PASS          0xaa
#define MESSAGE_ECHO_RESPONSE           0xee
#define MESSAGE_SELF_TEST_FAIL_1        0xfc
#define MESSAGE_SELF_TEST_FAIL_2        0xfd
#define MESSAGE_RESEND                  0xfe
#define MESSAGE_ERROR_2                 0xff

void keyboard_handle_press();

#endif /* _DRIVER_KEYBOARD_H */