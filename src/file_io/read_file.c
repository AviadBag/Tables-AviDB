#include "file_io/write_file.h"
#include "data_types/table.h"

#include <stdio.h>
#include <stdint.h>

#define HEADER_MAGIC 0x3910

#define ERROR 0
#define SUCCESS 1

#define FALSE 0
#define TRUE 1

// *body should be initialized.
int read_body(FILE* file, struct s_table_body* body)
{
    return SUCCESS;
}

// *header should be initialized.
int read_header(FILE* file, struct s_table_header* header)
{
    return SUCCESS;
}

int check_magic(FILE* file)
{
    uint16_t magic;
    if (!fread(&magic, 2, 1, file)) return ERROR;
    fseek(file, 2, SEEK_CUR);
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