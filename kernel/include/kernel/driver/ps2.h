/*====-------------------- ps2.h - PS/2 driver header --------------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef DRIVER_PS2_H
#define DRIVER_PS2_H

#include <kernel/driver/ports.h>

/* PS/2 controller status */
#define PS2_STATUS_OUT_BUFFER   0b00000001
#define PS2_STATUS_IN_BUFFER    0b00000010
#define PS2_STATUS_SYSTEM_FLAG  0b00000100
#define PS2_STATUS_COMMAND_DATA 0b00001000
#define PS2_STATUS_TIMEOUT_ERR  0b01000000
#define PS2_STATUS_PARITY_ERR   0b10000000

#define PS2_STATUS_BUFFER_EMPTY 0
#define PS2_STATUS_BUFFER_FULL  1
#define PS2_STATUS_ERROR        1

/* PS/2 controller commands */
#define PS2_CMD_READ_BYTE_0             0x20
#define PS2_CMD_READ_BYTE_N             0x21
#define PS2_CMD_WRITE_BYTE_0            0x60
#define PS2_CMD_WRITE_BYTE_N            0x61
#define PS2_CMD_DISABLE_SECOND_PORT     0xa7
#define PS2_CMD_ENABLE_SECOND_PORT      0xa8
#define PS2_CMD_TEST_SECOND_PORT        0xa9
#define PS2_CMD_CONTROLLER_SELF_TEST    0xaa
#define PS2_CMD_TEST_FIRST_PORT         0xab
#define PS2_CMD_DIAGNOSTIC_DUMP         0xac
#define PS2_CMD_DISABLE_FIRST_PORT      0xad
#define PS2_CMD_ENABLE_FIRST_PORT       0xae
#define PS2_CMD_READ_CTRL_INPUT_PORT    0xc0
#define PS2_CMD_COPY_INPUT_LOW          0xc1
#define PS2_CMD_COPY_INPUT_HIGH         0xc2
#define PS2_CMD_READ_CTRL_OUTPUT        0xd0
#define PS2_CMD_WRITE_NEXT_CTRL_OUT     0xd1
#define PS2_CMD_WRITE_NEXT_FIRST_OUT    0xd2
#define PS2_CMD_WRITE_NEXT_SECOND_OUT   0xd3
#define PS2_CMD_WRITE_NEXT_SECOND_IN    0xd4
#define PS2_CMD_PULSE_OUTPUT_LOW        0xf0

#define PS2_TEST_OK     0x55
#define PS2_TEST_FAIL   0xfc

/* PS/2 controller config bits */
#define PS2_CONFIG_FIRST_INT_ENABLE     0b00000001
#define PS2_CONFIG_SECOND_INT_ENABLE    0b00000010
#define PS2_CONFIG_SYSTEM_FLAG          0b00000100
#define PS2_CONFIG_FIRST_PORT_CLOCK     0b00010000
#define PS2_CONFIG_SECOND_PORT_CLOCK    0b00100000
#define PS2_CONFIG_FIRST_PORT_TRANS     0b01000000

/* outgoing commands to PS/2 devices */
#define PS2_DEV_COMMAND_ECHO            0xee
#define PS2_DEV_COMMAND_IDENTIFY        0xf2
#define PS2_DEV_COMMAND_ENABLE_SCAN     0xf4
#define PS2_DEV_COMMAND_DISABLE_SCAN    0xf5
#define PS2_DEV_COMMAND_RESEND          0xfe
#define PS2_DEV_COMMAND_RESET           0xff

/* incoming messages from PS/2 devices */
#define PS2_MESSAGE_SELF_TEST_OK        0xaa
#define PS2_MESSAGE_ECHO_RESPONSE       0xee
#define PS2_MESSAGE_ACKNOWLEDGE         0xfa
#define PS2_MESSAGE_SELF_TEST_FAIL_1    0xfc
#define PS2_MESSAGE_SELF_TEST_FAIL_2    0xfd
#define PS2_MESSAGE_RESEND              0xfe
#define PS2_MESSAGE_ERROR               0xff

void ps2_init(void);

#endif /* DRIVER_PS2_H */