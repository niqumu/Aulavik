/*====-------------- user.h - Aulavik API User/UI functions --------------====*\
 *
 * This file is a part of the Aulavik project. The Aulavik project is free
 * software, licenced under the MIT License.
 *
 * Usage of these works (including, yet not limited to, reuse, modification,
 * copying, distribution, and selling) is permitted, provided that the relevant
 * copyright notice and permission notice (as specified in LICENSE) shall be
 * included in all copies or substantial portions of this software and all
 * documentation files.
 *
 * These works are provided "AS IS" with absolutely no warranty of any kind,
 * either expressed or implied.
 *
 * You should have received a copy of the MIT License alongside this software;
 * refer to LICENSE for information. If not, refer to https://mit-license.org.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _AULAVIK_USER_H
#define _AULAVIK_USER_H 1

#include <sys/cdefs.h>

#include <stdint.h>

typedef uint32_t WindowHandle;

__BEGIN_DECLS

WindowHandle CreateWindow(const char *title);

void DestroyWindow(WindowHandle window);

void ShowWindow(WindowHandle window);

void HideWindow(WindowHandle window);

void SetWindowSize(WindowHandle window, int w, int h, int *dw, int *dh);

void SetWindowPosition(WindowHandle window, int x, int y);

void GetWindowSizePos(WindowHandle window, int *x, int *y, int *w, int *h);

__END_DECLS

#endif /* _AULAVIK_USER_H */
