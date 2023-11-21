/*====------------ strcat.c - string.h strcat implementation -------------====*\
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

#include <string.h>

/**
 * Appends a copy of the string pointed to by src onto the end of the string
 * pointed to by dest. The terminating null-character of dest is overwritten.
 * The two strings should not overlap.
 * @param dest A pointer to a string to append onto
 * @param src A pointer to a string to append to dest
 * @return The value of dest
 */
char* strcat(char *dest, const char *src)
{
	char *ptr = dest + strlen(dest);

	while (*src)
		*ptr++ = *src++;

	*ptr = 0;
	return dest;
}