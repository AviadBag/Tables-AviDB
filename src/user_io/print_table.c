#include "user_io/print_table.h"
#include "file_io/table.h"

#include <stdio.h>

static void print_table_name(char* table_name)
{
        printf("Table Name: %s\n", table_name);
}

static char* type_to_string(enum e_type type)
{
    switch (type)
    {
        case (STRING): return "String";
        case (INTEGER): return "Integer";
        case (ERROR): return "Error";
        default: return "Unknown";
    }
}

static void print_table_columns_structure(struct s_columns_structure columns_structure)
{
    int length = columns_structure.length;
    if (length == 0) // No columns
    {
        printf("No Columns.\n");
        return; 
    }

    printf("Columns Structure: ");
    for (int i = 0; i < length-1; i++)
    {
        enum e_type type = columns_structure.columns_types[i];
        printf("[%d] = %s, ", i, type_to_string(type));
    }

    enum e_type last_type = columns_structure.columns_types[length-1];
    printf("[%d] = %s\n", length-1, type_to_string(last_type));
}

void print_table_header(struct s_table_header header)
{
    print_table_name(header.table_name);
    print_table_columns_structure(header.columns_structure);
}

void print_row(struct s_row row)
{

}

void print_table_body(struct s_table_body body)
{
    int length = body.length;

    for (int i = 0; i < length; i++) print_row(body.rows[length]);
}

void print_table(struct s_table table)
{
    print_table_header(table.table_header);
    print_table_body(table.table_body);
}