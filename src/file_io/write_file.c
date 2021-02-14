#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "file_io/write_file.h"
#include "data_types/table.h"

#define HEADER_MAGIC 0x3910

int write_column(FILE* file, struct s_column column)
{
    enum e_type type = column.type;
    char* name = column.name;

    // Write type
    if (fwrite(&type, 1, 1, file) != 1) 
        return 0; // ERROR

    // Write name
    if (type != ERROR)
    {
        int length = strlen(name) + 1; // Plus one for null terminator.
        if (fwrite(name, length, 1, file) != 1)
            return 0; // ERROR
    }

    return 1;
}

int write_columns_structure(FILE* file, struct s_columns_structure columns_structure)
{
    int length = columns_structure.length;
    for (int i = 0; i < length; i++)
    {
        struct s_column column = columns_structure.columns[i];
        if (write_column(file, column) == 0) 
            return 0; // ERROR
    }

    return 1;
}

int write_table_header(FILE* file, struct s_table_header table_header)
{
    // Write magic
    uint16_t magic = HEADER_MAGIC;
    if (fwrite(&magic, 2, 1, file) != 1) 
        return 0; // ERROR

    // Write DB name
    int length = strlen(table_header.table_name)+1; // Plus one for null terminator
    if (fwrite(table_header.table_name, length, 1, file) != 1)
        return 0; // ERROR

    // Write DB columns
    if (write_columns_structure(file, table_header.columns_structure) == 0)
        return 0; // ERROR

    return 1;
}

int write_table_body(FILE* file, struct s_table_body table_body)
{
    return 0;
}

// Returns zero if there is any error; Else, return non-zero number.
int write_file(struct s_table table, char* file_name)
{
    FILE* file = fopen(file_name, "wb");
    if (file == NULL) return 0;
    
    if (write_table_header(file, table.table_header) == 0 || write_table_body(file, table.table_body))
        return 0; // ERROR

    return 1;
}  