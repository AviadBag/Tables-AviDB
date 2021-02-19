#ifndef DATA_TYPES_H
#define DATA_TYPES_H

typedef unsigned char byte;

enum e_type
{
    STRING = 1,
    INTEGER = 2,
};

union u_data
{
    char* string;
    int   integer;
};

struct s_data_holder
{
    enum e_type type;
    union u_data data;
};

#endif