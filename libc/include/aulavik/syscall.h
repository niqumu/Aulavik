/*====------------------- syscall.h - Aulavik syscalls -------------------====*\
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

#ifndef _AULAVIK_SYSCALL_H
#define _AULAVIK_SYSCALL_H 1

#include <stddef.h>
#include <sys/types.h>

#ifdef __STRICT_ANSI__
  #define ASM(n) __asm__ __volatile__ (n)
#else
  #define ASM(n) asm volatile (n)
#endif /* __STRICT_ANSI__ */

#define SYSCALL_KERNEL_ERR      1
#define SYSCALL_ERR             (-1)

#define SYSCALL_READ    0
#define SYSCALL_WRITE   1
#define SYSCALL_EXIT    60

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * writes up to count bytes from the buffer, starting at buf, to the file
 * referred to by the file descriptor fd.
 */
ssize_t syscall_write(unsigned int fd, const void *buf, size_t count);

_Noreturn void syscall_exit(int status);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* _AULAVIK_SYSCALL_H */
