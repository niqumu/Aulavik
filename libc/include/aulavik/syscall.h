/*====------------------- syscall.h - Aulavik syscalls -------------------====*\
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
 * You should have received a copy of the MIT License alongside this software,
 * refer to LICENSE for more information. If not, please refer to
 * https://mit-license.org.
 *
\*====--------------------------------------------------------------------====*/

#ifndef _AULAVIK_SYSCALL_H
#define _AULAVIK_SYSCALL_H 1

#include <sys/cdefs.h>

#include <stddef.h>
#include <stdio.h>

#include <sys/types.h>

#define SYSCALL_KERNEL_ERR      1
#define SYSCALL_ERR             (-1)

#define SYSCALL_READ    0
#define SYSCALL_WRITE   1
#define SYSCALL_EXIT    60

__BEGIN_DECLS

/*
 * writes up to count bytes from the buffer, starting at buf, to the file
 * referred to by the file descriptor file.
 */
ssize_t syscall_write(const FILE *file, const void *buf, size_t count);

_Noreturn void syscall_exit(int status);

__END_DECLS

#endif /* !_AULAVIK_SYSCALL_H */
