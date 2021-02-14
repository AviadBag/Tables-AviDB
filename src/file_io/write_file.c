#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "file_io/write_file.h"
#include "data_types/table.h"

#define HEADER_MAGIC 0x3910

int write_string(FILE* file, char* string)
{
    int length = strlen(string) + 1; // Plus one for null terminator.
    if (fwrite(string, length, 1, file) != 1)
        return 0; // ERROR

    return 1;
}

int write_column(FILE* file, struct s_column column)
{
    enum e_type type = column.type;
    char* name = column.name;

    // Write type
    if (fwrite(&type, 1, 1, file) != 1) 
        return 0; // ERROR

    // Write name
    if (type != ERROR)
        return write_string(file, name);

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

    char* table_name = table_header.table_name;
    if (write_string(file, table_name) == 0)
        return 0; // ERROR

    // Write DB columns
    if (write_columns_structure(file, table_header.columns_structure) == 0)
        return 0; // ERROR

    return 1;
}

int write_field(FILE* file, struct s_data_holder field)
{
    if (field.type == STRING) 
        return write_string(file, field.data.string);
    else if (field.type == INTEGER)
        return fwrite(&field.data.integer, sizeof(int), 1, file) != 0;
    else
    {
        char null = 0;
        return fwrite(&null, sizeof(char), 1, file) != 0;
    }
}

int write_row(FILE* file, struct s_row row)
{
    for (int i = 0; i < row.length; i++)
    {
        if (write_field(file, row.fields[i]) == 0) 
            return 0; // ERROR
    }
    
    return 1;
}

int write_table_body(FILE* file, struct s_table_body table_body)
{
    for (int i = 0; i < table_body.length; i++)
    {
        if (write_row(file, table_body.rows[i]) == 0) 
            return 0; // ERROR
    }
    
    return 1;
}

// Returns zero if there is any error; Else, return non-zero number.
int write_file(struct s_table table, char* file_name)
{
    FILE* file = fopen(file_name, "wb");
    if (file == NULL) return 0;
    
    if (write_table_header(file, table.table_header) == 0 
        || 
        write_table_body(file, table.table_body) == 0
    )
        return 0; // ERROR

    return 1;
}  