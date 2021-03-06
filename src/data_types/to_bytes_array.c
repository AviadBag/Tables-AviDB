#include "data_types/to_bytes_array.h"
#include <stdlib.h>
#include <string.h>

/** 
 * @brief Convert string to bytes array
 * 
 * @details
 * This method converts a string to bytes array.
 * The returned array should be explicity freed.
 * 
 * @param string The string to convert.
 * 
 * @return Zero if there is any error; A pointer to the bytes array if not.
 */
static byte* string_to_bytes_array(char* str)
{
    byte* array = (byte*) malloc(strlen(str) * sizeof(char));

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
 * 
 * @param integer The integer to convert.
 * 
 * @return Zero if there is any error; A pointer to the bytes array if not.
 */
static byte* integer_to_bytes_array(int n)
{
    byte* array = (byte*) malloc(sizeof(int) * sizeof(byte));
    
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
 * 
 * @param data_holder The data_holder to convert.
 * 
 * @return Zero if there is any error; A pointer to the bytes array if not.
 */
byte* to_bytes_array(struct s_data_holder data_holder)
{
    switch (data_holder.type)
    {
        case STRING:
            return string_to_bytes_array(data_holder.data.string);

        case INTEGER:
            return integer_to_bytes_array(data_holder.data.integer);

        default:
            return (byte*) 0;
    }
}