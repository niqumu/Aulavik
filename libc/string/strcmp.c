/*====------------ strcmp.c - string.h strcmp implementation -------------====*\
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
