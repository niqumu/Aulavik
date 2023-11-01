#include <string.h>

/****************************************************************************
 * Function: copies the value of value (casted to an unsigned char) into 
 *      each of the first n characters of the object pointed to by dest_ptr.
 *
 * Return: The value of dest_ptr
 ***************************************************************************/
 void* memset(void *dest_ptr, int value, size_t n)
 {
        unsigned char *dest = (unsigned char*) dest_ptr;

        for (size_t i = 0; i < n; i++)
                dest[i] = (unsigned char) value;

        return dest_ptr;
}
