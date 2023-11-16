/*====---------------- rainier.h - Rainier window manager ----------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that the relevant copyright 
 * notice and permission notice shall be included in all copies or substantial 
 * portions of this software and all documentation files.
 * 
 * Refer to LICENSE for more information. These works are provided with 
 * absolutely no warranty.
 * 
\*====--------------------------------------------------------------------====*/

#ifndef RAINIER_H
#define RAINIER_H

#define RAINIER_DEBUGGING_ELEMENTS

#include <kernel/driver/mouse.h>

struct rainier_window* rainier_get_windows(void);

struct rainier_window* rainier_get_focused_window(void);

struct render_context* rainier_get_background_ctx(void);

void rainier_set_focused_window(struct rainier_window *window);

void rainier_tick();

void rainier_process_mouse(struct mouse_packet packet);

void rainier_main();

#endif /* RAINIER_H */
