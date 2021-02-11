#ifndef TABLE_H
#define TABLE_H

#include "data_types/data_types.h"

struct s_row
{
    struct s_data_holder* fields;
    int length; // The length of the fields array.
};

struct s_columns_structure
{
    enum e_type* columns_types;
    int length; // The length of the columns array.
};

struct s_table_body
{
    struct s_row* rows;
    int length; // The length of the rows array.
};

struct s_table_header
{
    char* table_name;
    struct s_columns_structure columns_structure;
};


struct s_table
{
    struct s_table_header table_header;
    struct s_table_body table_body;
};

#endif