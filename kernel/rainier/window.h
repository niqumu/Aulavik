/*====-------- window.h - Rainier window structures and functions --------====*\
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

#ifndef RAINIER_WINDOW_H
#define RAINIER_WINDOW_H

#include <stdbool.h>
#include <stdint.h>

#include <kernel/graphics/graphics.h>

#define WINDOW_AREA_NONE        0b00000000
#define WINDOW_AREA_TOP         0b00000001
#define WINDOW_AREA_BOTTOM      0b00000010
#define WINDOW_AREA_LEFT        0b00000100
#define WINDOW_AREA_RIGHT       0b00001000
#define WINDOW_AREA_TITLEBAR    0b00010000
#define WINDOW_AREA_CONTENT     0b00100000
#define WINDOW_AREA_MINIMIZE    0b01000000
#define WINDOW_AREA_CLOSE       0b10000000

#define WINDOW_TRAY_WIDTH       250
#define WINDOW_TRAY_HEIGHT      35

#define WINDOW_BORDER_SIZE      5
#define WINDOW_TITLEBAR_SIZE    25

#define WINDOW_MIN_WIDTH        300
#define WINDOW_MIN_HEIGHT       200
#define WINDOW_DEFAULT_WIDTH    700
#define WINDOW_DEFAULT_HEIGHT   500

#define WINDOW_MAX_TITLE_LENGTH 63

struct rainier_window {
	char *title;
	int x, y;
	int last_tray_y, last_tray_x;
	int width, height;
	bool minimized;

	uint32_t handle;
	bool awaiting_redraw;
	struct rainier_window *next_window;
	struct rainier_window *last_window;

	struct render_context ctx;
	struct render_context client_ctx;
};

/**
 * Iterates over all eligable windows to find the front window that is visible
 * and not minized.
 * @return The frontmost visible and restored window, or NULL if none exists
 */
struct rainier_window* window_find_front(void);

/**
 * Brings the provided window to the front of the desktop. Doing so will
 * remove the window from the window list, shuffle all windows in front
 * backwards to take its place, and put the window at the front.
 *
 * This function automatically gives focus to the window being brought
 * forward, and takes it away from the window previously in front. Both
 * windows are redrawn.
 *
 * @param window The window to bring to the front
 */
void window_bring_to_front(struct rainier_window *window);

void window_handle_drag(struct rainier_window *window, uint8_t flags,
                        int delta_x, int delta_y);

uint8_t window_locate_click(int x, int y, struct rainier_window window);

/**
 * Redraws the contents of the provided window, baking the latest contents
 * of the content buffer onto the master framebuffer for the window.
 * @param window The window to redraw
 */
void window_redraw_content(struct rainier_window *window);

/**
 * Redraws the provided window, drawing the frame and latest contents of the
 * content framebuffer onto the master framebuffer for the window.
 *
 * This function should only be called when the window has changed in such a
 * way that the frame must be redrawn, i.e. it was minimized/restored, resized,
 * lost/gained focus, etc. If only the contents have changed, the
 * window_redraw_content function should be called instead.
 *
 * @param window The window to redraw
 */
void window_redraw(struct rainier_window *window);

/**
 * Marks the provided window as needed to be repainted on the next frame. This
 * is preferrable to immediately redrawing the window, as there may be multiple
 * operations per frame that require the window to be redrawn, which only needs
 * to happen one time.
 * @param window The window to schedule repainting for
 */
void window_redraw_later(struct rainier_window *window);

/**
 * Restores (un-minimizes) the provided window. Doing so will place it on top
 * of all other windows and give it focus.
 * @param window The window to restore
 */
void window_restore(struct rainier_window *window);

/**
 * Minimizes the provided window. Doing so will find the new front window and
 * give it focus.
 * @param window The window the minimize
 */
void window_minimize(struct rainier_window *window);

/**
 * Resizes the provided window to the closest possible size to the provided
 * width and height. Doing so will cause the window to be redrawn.
 *
 * If the window cannot be scaled to the provided dimensions for whatever
 * reason (the new size is below the window's minimum size, the window doesn't
 * allow resizing, etc.), the window will be set to the closest possible size.
 *
 * The dw and dh pointers will have their value set to the change in width and
 * change in height that took place, respectively, as this is not guarenteed
 * to always be the actual difference betweeen the old size and requested size.
 *
 * @param window The window to resize
 * @param w The requested width to resize the window to
 * @param h The requested height to resize the window to
 * @param dw A pointer to the actual change in width that took place
 * @param dh A pointer to the actual change in height that took place
 */
void window_resize(struct rainier_window *window, int w, int h, int *dw, int *dh);

/**
 * Sets the title of the provided window to the provided string. If the title
 * provided exceeds the maximum permitted length, it is automatically
 * truncated, and ellipses are placed at the end to indicate to the user that
 * this truncation took place.
 * @param window The window to set the title of
 * @param title The new title the window should display
 */
void window_set_title(struct rainier_window *window, char *title);

/**
 * Creates and returns a window with the given title and dimensions. Memory
 * is automatically allocated for the title and framebuffers. This function
 * does not register the window or add it to the window list.
 *
 * Once a window is created, it must be properly disgarded through the use of
 * the window_destroy function in order to free its memory.
 *
 * @param title The title of the window to create
 * @param width The preferred width of the window. If zero is passed, the
 *      system default window width is used instead.
 * @param height The preferred height of the window. If zero is passed, the
 *      system default window height is used instead.
 * @return A new window, ready to use, with the provided values
 */
struct rainier_window window_create(char *title, int width, int height);

/**
 * Destroys the provided window, freeing both of the window's framebuffers and
 * the window title. If the provided window had focus, it is given to the next
 * top window. This function does not de-register the window or remove it
 * from the window list.
 *
 * Windows should never be disgarded without using this function to properly
 * clean up and free any memory used by the window.
 *
 * @param window The window to destroy
 */
void window_destroy(struct rainier_window *window);

#endif /* RAINIER_WINDOW_H */
