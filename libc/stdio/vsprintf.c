/*====------------ vsprintf - stdio.h vsprintf implementation ------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <stdio.h>

#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

static bool print(char *dest, const char *src, size_t length)
{
	char *ptr = dest + strlen(dest);

	for (size_t i = 0; i < length; i++)
		*ptr++ = *src++;

	*ptr = 0;

	return true;
}

// TODO how big does the buffer really need to be?
char* convert(uint64_t number, int base, int min_digits)
{
	int digits = 0;
	static char chars[] = "0123456789abcdef";
	static char buff[64];
	char *ptr = &buff[63];

	*ptr = '\0';

	do {
		digits++;
		*--ptr = chars[number % base];
		number /= base;
	} while (number);

	if (digits < min_digits)
		ptr -= (min_digits - digits);

	return ptr;
}

int vsprintf(char *str, const char* restrict format, va_list parameters)
{
	int written = 0;
	int min_digits = 0;

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

			if (!print(str, format, amount))
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

				if (!print(str, &c, sizeof(c)))
					return -1;

				written++;
				break;
			case 'd':
				format++;
				const int num = va_arg(parameters, int);
				// TODO proper abs();
				char *d_str = convert(num * ((num > 0) - (num < 0)), 10, min_digits);
				len = strlen(d_str);

				if (maxleft < len) {
					// TODO set errno to EOVERFLOW
					return -1;
				}

				if (num < 0) {
					written++;
					char neg = '-';
					if (!print(str, &neg, sizeof(neg)))
						return -1;
				}

				if (!print(str, d_str, len))
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

				if (!print(str, s_str, len))
					return -1;

				written += len;
				break;
			case 'x':
				format++;
				const uint64_t num_hex = va_arg(parameters, int);
				char *x_str = convert(num_hex, 16, min_digits);
				len = strlen(x_str);

				if (maxleft < len) {
					// TODO set errno to EOVERFLOW
					return -1;
				}

				if (!print(str, "0x", sizeof("0x") - 1))
					return -1;

				if (!print(str, x_str, len))
					return -1;

				written += len + 2;
				break;
			default:
				if (*format >= '0' && *format <= '9') {
					format++;
					min_digits *= 10;
					min_digits += *format - '0';

					written++;
					break;
				}

				format = format_begun_at;
				len = strlen(format);

				if (maxleft < len) {
					// TODO set errno to EOVERFLOW
					return -1;
				}

				if (!print(str, format, len))
					return -1;

				written += len;
				format += len;
		}
	}

	return written;
}
