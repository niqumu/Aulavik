/*====------------- logger.h - Header for the kernel logger --------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _KERNEL_LOGGER_H
#define _KERNEL_LOGGER_H

/* remove this if serial logging is not desired */
#define SERIAL_LOGGING_ENABLED
#define SERIAL_LOGGING_PORT 0x3f8
#define SERIAL_LOGGING_ANSI /* remove if color is not desired over serial, */
			    /* ie, logging to a file instead of a terminal */

#define FG_GREEN        "\e[92m"
#define FG_ORANGE       "\e[93m"
#define FG_RED          "\e[91m"
#define FG_DARK_GRAY    "\e[90m"
#define FG_GRAY         "\e[37m"

#define PREFIX_DEBUG    "[ " FG_DARK_GRAY "DBG" FG_GRAY " ] "
#define PREFIX_DEBUG_M  "[ DBG ]"
#define PREFIX_OK       "[ " FG_GREEN "OK" FG_GRAY " ] "
#define PREFIX_OK_M     "[ OK ]"
#define PREFIX_WARN     "[ " FG_ORANGE "WARN" FG_GRAY " ] "
#define PREFIX_WARN_M   "[ WARN ]"
#define PREFIX_ERROR    "[ " FG_RED "ERR" FG_GRAY " ] "
#define PREFIX_ERROR_M  "[ ERR ]"

void k_print(const char* __restrict, ...);
void k_debug(const char* __restrict, ...);
void k_ok(const char* __restrict, ...);
void k_warn(const char* __restrict, ...);
void k_error(const char* __restrict, ...);

#endif /* _KERNEL_LOGGER_H */