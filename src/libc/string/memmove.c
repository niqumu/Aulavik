#include <string.h>

/****************************************************************************
 * Function: copies n characters from the object pointed to by src_ptr into
 *      the object pointed to by dest_ptr. 
 *
 * Return: The value of dest_ptr
 ***************************************************************************/
void* memmove(void *dest_ptr, const void *src_ptr, size_t n)
{
        unsigned char *dest = (unsigned char*) dest_ptr;
        const unsigned char *src = (const unsigned char*) src_ptr;

        if (dest < src) {
                for (size_t i = 0; i < n; i++)
                        dest[i] = src[i];
        } else {
                for (size_t i = n; i; i--)
                        dest[i - 1] = src[i - 1];
        }

        return dest_ptr;
}
