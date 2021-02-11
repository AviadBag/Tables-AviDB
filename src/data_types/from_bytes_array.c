#include "data_types/from_bytes_array.h"

/** 
 * @brief Convert string bytes array to data_holder
 * 
 * @param bytes_array The input bytes array.
 * 
 * @return A data_holder contains the data.
 */
static struct s_data_holder from_string_bytes_array(byte* bytes_array)
{
    struct s_data_holder data_holder;
    data_holder.type = STRING;
    
    union u_data data;
    data.string = (char*) bytes_array;
    data_holder.data = data;

    return data_holder;
}

/** 
 * @brief Convert integer bytes array to data_holder
 * 
 * @param bytes_array The input bytes array.
 * 
 * @return A data_holder contains the data.
 */
static struct s_data_holder from_integer_bytes_array(byte* bytes_array)
{
    int integer = 0;
    for (int i = sizeof(int)-1; i >= 0; i--)
    {
        if (integer == 0 && bytes_array[i] == 0) continue;
        integer += (int) bytes_array[i];
        if (i != 0) integer <<= 8;
    }

    struct s_data_holder data_holder;
    data_holder.type = INTEGER;
    
    union u_data data;
    data.integer = integer;
    data_holder.data = data;

    return data_holder;
}

/** 
 * @brief Convert bytes array to data_holder
 * 
 * @param type The type of the input bytes array.
 * @param bytes_array The input bytes array.
 * 
 * @return A data_holder contains the data. If there is any error, then ERROR data_holder is returned.
 */
struct s_data_holder from_bytes_array(enum e_type type, byte* bytes_array)
{
    switch (type)
    {
        case INTEGER:
            return from_integer_bytes_array(bytes_array);
        
        case STRING:
            return from_string_bytes_array(bytes_array);
    }

    struct s_data_holder error;
    error.type = ERROR;
    return error;
}