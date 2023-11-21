/*====------------ strlen.c - string.h malloc implementation -------------====*\
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
 * Computes the length of the string pointed to by str, defined as the number
 * of characters prior to a null-terminator.
 * @param str A pointer to the string to inspect
 * @return The amount of characters in the string before a null-character
 */
size_t strlen(const char *str)
{
	size_t len = 0;

	while (str[len])
		len++;

	return len;
}
