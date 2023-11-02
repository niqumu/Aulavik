#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>

#include <stdio.h>
#include <string.h>

static bool print(const char *string, size_t length)
{
        const unsigned char *bytes = (const unsigned char*) string;

        for (size_t i = 0; i < length; i++) {
                if (putchar(bytes[i]) == EOF)
                        return false;
        }

        return true;
}

// TODO how big does the buffer really need to be?
char* convert(unsigned int number, int base)
{
        static char chars[] = "0123456789abcdef";
        static char buff[64];
        char *ptr = &buff[63];

        *ptr = '\0';

        do {
                *--ptr = chars[number % base];
                number /= base;
        } while (number);

        return ptr;
}

// TODO this can probably be split up and optimized
int printf(const char* restrict format, ...)
{
        va_list parameters;
        va_start(parameters, format);

        int written = 0;

        while (*format) {
                size_t maxleft = INT_MAX - written;

                if (format[0] != '%' || format[1] == '%') {
                        if (format[0] == '%')
                                format++;

                        size_t amount = 1;

                        while (format[amount] && format[amount] != '%')
                                amount++;

                        if (maxleft < amount) {
                                // TODO set errno to EOVERFLOW
                                return -1;
                        }

                        if (!print(format, amount)) 
                                return -1;

                        format += amount;
                        written += amount;
                        continue;
                }

                const char *format_begun_at = format++;
                size_t len;

                switch (*format) {
                case 'c':
                        format++;
                        char c = (char) va_arg(parameters, int);

                        if (!maxleft) {
                                // TODO set errno to EOVERFLOW
                                return -1;
                        }

                        if (!print(&c, sizeof(c)))
                                return -1;

                        written++;
                        break;
                case 'd':
                        format++;
                        const int num = va_arg(parameters, int);
                        // TODO proper abs();
                        char *d_str = convert(num * ((num > 0) - (num < 0)), 10);
                        len = strlen(d_str);

                        if (maxleft < len) {
                                // TODO set errno to EOVERFLOW
                                return -1;
                        }

                        if (num < 0) {
                                written++;
                                char neg = '-';
                                if (!print(&neg, sizeof(neg)))
                                        return -1;
                        }

                        if (!print(d_str, len))
                                return -1;

                        written += len;
                        break;
                case 's':
                        format++;
                        const char *s_str = va_arg(parameters, const char*);
                        len = strlen(s_str);

                        if (maxleft < len) {
                                // TODO set errno to EOVERFLOW
                                return -1;
                        }

                        if (!print(s_str, len))
                                return -1;

                        written += len;
                        break;
                case 'x':
                        format++;
                        const int num_hex = va_arg(parameters, int);
                        char x_str = convert(num_hex, 16);
                        len = strlen(x_str);

                        if (maxleft < len) {
                                // TODO set errno to EOVERFLOW
                                return -1;
                        }

                        if (!print("0x", sizeof("0x") - 1))
                                return -1;

                        if (!print(x_str, len))
                                return -1;

                        written += len + 2;
                        break;
                default:
                        format = format_begun_at;
                        len = strlen(format);

                        if (maxleft < len) {
                                // TODO set errno to EOVERFLOW
                                return -1;
                        }

                        if (!print(format, len))
                                return -1;

                        written += len;
                        format += len;
                }
        }

        va_end(parameters);
        return written;
}
