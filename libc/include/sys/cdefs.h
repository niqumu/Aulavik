/*====-------------- cdefs.h - Aulavik libc declarations -----------------====*\
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

/* Define environment-appropriate macros for inline ASM */
#ifdef __STRICT_ANSI__
	#define ASM_VOLATILE(n) __asm__ __volatile__ (n)
	#define ASM(n) __asm__(n)
#else
	#define ASM_VOLATILE(n) asm volatile (n)
	#define ASM(n) asm(n)
#endif /* __STRICT_ANSI__ */

/*
 * In supported environments, warn callers when a function's return value is
 * not used. This should only be used when ignoring the return value will result
 * in a bug; such as ignoring the result of memory allocation functions.
 */
#if __has_attribute(warn_unused_result)
	#define __ensure_result_use __attribute__((__warn_unused_result__))
#else
	#define __ensure_result_use
#endif /* warn_unused_result */


#endif /* !_SYS_CDEFS_H */
