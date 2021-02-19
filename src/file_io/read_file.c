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
    if (fread(*string_ptr, sizeof(char), string_length, file) < string_length) 
        return ERROR;
    *(*string_ptr + string_length) = 0; // Null terminator

    return SUCCESS;
}

// *header should be initialized.
e_status read_header(FILE* file, struct s_table_header* header)
{
    char* table_name;
    if (read_string_from_file(file, &table_name) == ERROR) return ERROR;
    header->table_name = table_name;

    return SUCCESS;
}


// *body should be initialized.
e_status read_body(FILE* file, struct s_table_body* body)
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

// *table should be initialized.
e_status read_file(char* file_name, struct s_table* table)
{
    FILE* file = fopen(file_name, "rb");
    if (file == NULL) return ERROR;

    if (check_magic(file) == FALSE)
        goto close_and_error;
    
    struct s_table_header header;
    if (read_header(file, &header) == ERROR) goto close_and_error;;
    
    struct s_table_body body;
    if (read_body(file, &body) == ERROR) goto close_and_error;;

    table->table_header = header;
    table->table_body = body;

    fclose(file);
    return SUCCESS;

    close_and_error:
        fclose(file);
        return ERROR;
}