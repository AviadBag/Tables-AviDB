#include "file_io/write_file.h"
#include "data_types/table.h"
#include "define/define.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define HEADER_MAGIC 0x3910

e_status get_string_length(FILE* file, int* length)
{
    *length = 0;
    char current = 1;
    do
    {
        if (fread(&current, sizeof(char), 1, file) < sizeof(char)) 
            return ERROR;

        (*length)++;
    } while (current != 0);

    (*length)--; // Don't include null terminator

    return SUCCESS;
}

// string_ptr should be explicity freed.
e_status read_string_from_file(FILE* file, char** string_ptr)
{   
    int string_length;
    if (get_string_length(file, &string_length) == ERROR) return ERROR;

    // Take the file pointer back
    if (fseek(file, sizeof(char) * (string_length+1) * -1, SEEK_CUR) != 0) return ERROR;

    // Allocate memory for the string
    *string_ptr = (char* ) malloc(sizeof(char) * (string_length + 1));
    if (fread(*string_ptr, sizeof(char), string_length+1, file) < string_length) 
        return ERROR;

    return SUCCESS;
}

e_status read_int_from_file(FILE* file, int* n)
{
    if (fread(n, sizeof(int), 1, file) < 1) 
        return ERROR;

    return SUCCESS;
}

// column->name should be explicity freed.
e_status read_column(FILE* file, struct s_column* column)
{
    int type;
    if (read_int_from_file(file, &type) == ERROR)
        return ERROR;
    column->type = type;

    char* name;
    if (read_string_from_file(file, &name) == ERROR)
        return ERROR;
    column->name = name;

    return SUCCESS;
}

// columns_structure->columns should be explicity freed.
e_status read_columns_structure(FILE* file, struct s_columns_structure* columns_structure)
{
    int length;
    if (read_int_from_file(file, &length) == ERROR)
        return ERROR;
    columns_structure->length = length;

    struct s_column* columns = malloc(sizeof(struct s_column) * length);
    if (columns == NULL) return ERROR;

    for (int i = 0; i < length; i++)
    {
        struct s_column column;
        if (read_column(file, &column) == ERROR)
            return ERROR;

        columns[i] = column;
    }

    columns_structure->columns = columns;

    return SUCCESS;
}

// *header should be initialized.
e_status read_header(FILE* file, struct s_table_header* header)
{
    char* table_name;
    if (read_string_from_file(file, &table_name) == ERROR) return ERROR;
    header->table_name = table_name;

    struct s_columns_structure columns_structure;
    if (read_columns_structure(file, &columns_structure) == ERROR)
        return ERROR;
    header->columns_structure = columns_structure;

    return SUCCESS;
}


// *body should be initialized.
e_status read_body(FILE* file, struct s_columns_structure columns_structure, struct s_table_body* body)
{
    return SUCCESS;
}

e_bool check_magic(FILE* file)
{
    uint16_t magic;
    if (fread(&magic, 2, 1, file) < 1) 
        return FALSE;
    
    if (magic != HEADER_MAGIC) 
        return FALSE;

    return TRUE;
}

// The given table must be a table generated by read_file()!
void free_table(struct s_table table)
{
    free(table.table_header.table_name);
    for (int i = 0; i < table.table_header.columns_structure.length; i++)
        free(table.table_header.columns_structure.columns[i].name);
}

// *table must be initialized.
e_status read_file(char* file_name, struct s_table* table)
{
    FILE* file = fopen(file_name, "rb");
    if (file == NULL) return ERROR;

    if (check_magic(file) == FALSE)
        goto close_and_error;
    
    struct s_table_header header;
    if (read_header(file, &header) == ERROR) goto close_and_error;;
    
    struct s_table_body body;
    if (read_body(file, header.columns_structure, &body) == ERROR) goto close_and_error;;

    table->table_header = header;
    table->table_body = body;

    fclose(file);
    return SUCCESS;

    close_and_error:
        fclose(file);
        return ERROR;
}