#include "file_io/write_file.h"
#include "data_types/table.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define HEADER_MAGIC 0x3910

#define ERROR 0
#define SUCCESS 1

#define FALSE 0
#define TRUE 1

int get_string_length(FILE* file, int* length)
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
int read_string_from_file(FILE* file, char** string_ptr)
{   
    int string_length;
    get_string_length(file, &string_length);

    // Take the file pointer back
    fseek(file, sizeof(char) * (string_length+1) * -1, SEEK_CUR);

    // Allocate memory for the string
    *string_ptr = (char* ) malloc(sizeof(char) * (string_length + 1));
    fread(*string_ptr, sizeof(char), string_length, file);
    *(*string_ptr + string_length) = 0; // Null terminator
}

// *header should be initialized.
int read_header(FILE* file, struct s_table_header* header)
{
    char* table_name;
    if (!read_string_from_file(file, &table_name)) return ERROR;
    header->table_name = table_name;

    return SUCCESS;
}


// *body should be initialized.
int read_body(FILE* file, struct s_table_body* body)
{
    return SUCCESS;
}

int check_magic(FILE* file)
{
    uint16_t magic;
    if (!fread(&magic, 2, 1, file)) return ERROR;
    
    if (magic != HEADER_MAGIC) 
        return FALSE;

    return TRUE;
}

// *table should be initialized.
int read_file(char* file_name, struct s_table* table)
{
    FILE* file = fopen(file_name, "rb");
    if (file == NULL) return ERROR;

    if (!check_magic(file))
        return FALSE;
    
    struct s_table_header header;
    if (!read_header(file, &header)) return ERROR;
    
    struct s_table_body body;
    if (!read_body(file, &body)) return ERROR;

    table->table_header = header;
    table->table_body = body;

    return SUCCESS;
}