#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "symbol.h"
#include "parser.h"
#include "extern.h"

void firstPass(const char *input_file, struct Symbol *symbol_table, struct Extern *extern_table, int *passed_first, int *is_ent, int *is_ext)
{

    /* Openes the input file for reading and checks for errors when opening the file */
    FILE *assembly_file = fopen(input_file, "r");
    if (assembly_file == NULL)
    {
        fprintf(stderr, "Error opening the assembly file.\n");
        return;
    }

    parseFirstPass(assembly_file, symbol_table, extern_table, passed_first, is_ent, is_ext);

    fclose(assembly_file);
}