/*====------------ strlen.c - string.h malloc implementation -------------====*\
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
 * Computes the length of str until a null character is reached.
 * @param str The string to inspect
 * @return The amount of characters in str before a null-character
 */
size_t strlen(const char *str)
{
        size_t len = 0;

        while (str[len])
                len++;

        return len;
}
