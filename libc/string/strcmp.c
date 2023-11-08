/*====------------ strcmp.c - string.h strcmp implementation -------------====*\
 *
 * This code is a part of the Aulavik project.
 * Usage of these works is permitted provided that this instrument is retained
 * with the works, so that any entity that uses the works is notified of this
 * instrument. These works are provided without any warranty.
 *
\*====--------------------------------------------------------------------====*/

#include <string.h>

int strcmp(const char *str1, const char *str2)
{
	int i = 0;
	for (; str1[i] && str2[i]; i++)
		;

	return str1[i] - str2[i];
}