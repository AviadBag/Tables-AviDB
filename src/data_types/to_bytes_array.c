#include "data_types/to_bytes_array.h"
#include <stdlib.h>
#include <string.h>

/** 
 * @brief Convert string to bytes array
 * 
 * @details
 * This method converts a string to bytes array.
 * The returned array should be explicity freed.
 * If there is any error, then zero is returned. Else, non zero value is returned.
 * 
 * @param string The string to convert.
 * @param length This pointer will be filled with the length of the returned array.
 * 
 * @return Zero if there is any error; A pointer to the bytes array if not.
 */
static byte* string_to_bytes_array(char* str, int* length)
{
    *length = strlen(str);
    byte* array = (byte*) malloc(*length * sizeof(char));

    if (array == NULL) return (byte*) 0;

    strcpy((char*) array, str);

    return array;
}

/** 
 * @brief Convert integer to bytes array
 * 
 * @details
 * This method converts a integer to bytes array.
 * The returned array should be explicity freed.
 * If there is any error, then zero is returned. Else, non zero value is returned.
 * 
 * @param integer The integer to convert.
 * @param length This pointer will be filled with the length of the returned array.
 * 
 * @return Zero if there is any error; A pointer to the bytes array if not.
 */
static byte* integer_to_bytes_array(int n, int* length)
{
    *length = sizeof(int);
    byte* array = (byte*) malloc(*length * sizeof(byte));
    
    if (array == NULL) return (byte*) 0;

    for (int i = 0; i < sizeof(int); i++)
    {
        array[i] = (char) (n & 0xFF);
        n >>= 8;
    }

    return array;
}

/** 
 * @brief Convert data_holder to bytes array
 * 
 * @details
 * This method converts a data_holder to bytes array.
 * The returned array should be explicity freed.
 * If there is any error, then zero is returned. Else, non zero value is returned.
 * 
 * @param data_holder The data_holder to convert.
 * @param length This pointer will be filled with the length of the returned array.
 * 
 * @return Zero if there is any error; A pointer to the bytes array if not.
 */
byte* to_bytes_array(struct s_data_holder data_holder, int* length)
{
    switch (data_holder.type)
    {
        case STRING:
            return string_to_bytes_array(data_holder.data.string, length);

        case INTEGER:
            return integer_to_bytes_array(data_holder.data.integer, length);

        default:
            return (byte*) 0;
    }
}