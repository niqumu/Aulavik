/*====-------------- cdefs.h - Aulavik libc declarations -----------------====*\
 *
 * This code is a part of the Aulavik project. The Aulavik project is licenced
 * under the MIT License.
 *
 * Usage of these works (including, yet not limited to, reuse, modification,
 * copying, distribution, and selling) is permitted provided that the relevant
 * copyright notice and permission notice (as specified in LICENSE) shall be
 * included in all copies or substantial portions of this software and all
 * documentation files.
 *
 * Refer to LICENSE for more information. These works are provided "AS IS" with
 * absolutely no warranty of any kind.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _SYS_CDEFS_H
#define _SYS_CDEFS_H 1

#define __AULAVIK_LIBC 1

/*
 * Versions of GCC prior to 9 will automatically insert extern "C" unless we
 * explicitly say not to. We don't want this, since we do it ourselves.
 */
#if __GNUC__ < 9
	#undef  NO_IMPLICIT_EXTERN_C
	#define NO_IMPLICIT_EXTERN_C 1
#endif /* __GNUC__ < 9 */

/* Define environment-appropriate macros for function declarations */
#ifdef __cplusplus
	#define __BEGIN_DECLS   extern "C" {
	#define __END_DECLS     } /* extern "C" */
#else
	#define __BEGIN_DECLS
	#define __END_DECLS
#endif /* __cplusplus */

#endif /* _SYS_CDEFS_H */
