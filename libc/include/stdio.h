#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h> /* va_list */

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);
int vprintf(const char* __restrict, va_list args);
int vsprintf(char* __restrict str, const char* __restrict, va_list args);

#ifdef __cplusplus
}
#endif

#endif // _STDIO_H
