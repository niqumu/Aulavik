/*====------------ strcmp.c - string.h strcmp implementation -------------====*\
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
 * Compares the string pointed to by str1 to the string pointed to by str2. If
 * the two strings are equal, the function will return zero. Otherwise, the
 * function will return the difference between the two characters where the
 * strings first differ.
 * @param str1 The string to compare against str2
 * @param str2 The string to compare against str1
 * @return Zero if the two strings are equal, or the difference between the
 *      characters where the strings first differ.
 */
int strcmp(const char *str1, const char *str2)
{
	int i = 0;
	for (; str1[i] && str2[i]; i++)
		;

	return str1[i] - str2[i];
}
