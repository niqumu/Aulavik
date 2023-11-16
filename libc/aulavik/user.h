/*====-------------- user.h - Aulavik API User/UI functions --------------====*\
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

#ifndef _AULAVIK_USER_H
#define _AULAVIK_USER_H

#include <stdint.h>

typedef uint64_t WindowHandle;

WindowHandle CreateWindow(const char *title);

void ShowWindow(WindowHandle window);

void HideWindow(WindowHandle window);

void SetWindowSize(WindowHandle window, int w, int h, int *dw, int *dh);

void SetWindowPosition(WindowHandle handle, int x, int y);

void GetWindowSizePos(WindowHandle handle, int *x, int *y, int *w, int *h);

#endif /* _AULAVIK_USER_H */
