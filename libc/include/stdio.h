#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h> /* va_list */

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict str, ...);
int putchar(int ch);
int puts(const char* str);
int sprintf(char* __restrict dest, const char* __restrict str, ...);
int vprintf(const char* __restrict str, va_list args);
int vsprintf(char* __restrict dest, const char* __restrict str, va_list args);

#ifdef __cplusplus
}
#endif

#endif // _STDIO_H
