#include <string.h>

/*****************************************************************************
 * Function: compares the first n characters of the object pointed to by
 *      ptr_a to the first n characters of the object pointed to by ptr_b.
 *
 * Return: An integer greater than, equal to, or less than zero, accordingly
 *      as the object pointed to by ptr_a is greater than, equal to, or less
 *      than the object pointed to by ptr_b
 ****************************************************************************/
int memcmp(const void *ptr_a, const void *ptr_b, size_t n)
{
        const unsigned char *a = (const unsigned char*) ptr_a;
        const unsigned char *b = (const unsigned char*) ptr_b;

        for (size_t i = 0; i < n; i++) {
                if (a[i] < b[i])
                        return -1;
                else if (b[i] < a[i])
                        return 1;
        }

        return 0;
}
