#include "user_io/print_table.h"
#include "data_types/table.h"

#include <stdio.h>
#include <stdlib.h>

#define INT_DECIMAL_STRING_SIZE(int_type) ((CHAR_BIT*sizeof(int_type)-1)*10/33+3)

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

static char* int_to_string(int n) // The result should be freed!
{
    int length = snprintf(NULL, 0, "%d", n);
    char* str = malloc(length + 1); // + 1 to keep place for the null terminator
    snprintf(str, length + 1, "%d", n);

    return str;
}

static char* data_holder_to_string(struct s_data_holder data_holder, int* shouldFree)
{
    *shouldFree = 0;

    if (data_holder.type == STRING) return data_holder.data.string;
    else if (data_holder.type == ERROR) return "Error";
    else if (data_holder.type == INTEGER)
    {
            char* str = int_to_string(data_holder.data.integer);
            *shouldFree = 1;
            return str;
    } else return "Unknown type";
}

static void print_table_columns_structure(struct s_columns_structure columns_structure)
{
    int length = columns_structure.length;
    if (length == 0) // No columns
    {
        printf("No Columns.\n");
        return; 
    }

    printf("Columns Structure:\n");
    for (int i = 0; i < length-1; i++)
    {
        struct s_column column = columns_structure.columns[i];
        printf("[%d] => %s: %s, ", i+1, column.name, type_to_string(column.type));
    }

    struct s_column last_column = columns_structure.columns[length-1];
    printf("[%d] => %s: %s\n", length, last_column.name, type_to_string(last_column.type));
}

static void print_table_header(struct s_table_header header)
{
    print_table_name(header.table_name);
    print_table_columns_structure(header.columns_structure);
}

static void print_row(struct s_row row)
{
    int length = row.length;
    for (int i = 0; i < row.length; i++)
    {
        int shouldFree;
        char* str = data_holder_to_string(row.fields[i], &shouldFree);
        printf("|  %s  ", str);

        if (shouldFree) free(str);
    }
    printf("|\n");
}

static void print_table_body(struct s_table_body body)
{
    int length = body.length;
    if (length == 0)
    {
        printf("No Body\n");
        return;
    }

    printf("Table Body:\n");

    for (int i = 0; i < length; i++) 
        print_row(body.rows[0]);
}

void print_table(struct s_table table)
{
    print_table_header(table.table_header);
    print_table_body(table.table_body);
}