#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "symbol.h"
#include "parser.h"
#include "extern.h"

void firstPass(const char *input_file, struct Symbol *symbol_table, struct Extern *extern_table, int *passed_first)
{

    /* Openes the input file for reading and checks for errors when opening the file */
    FILE *assembly_file = fopen(input_file, "r");
    if (assembly_file == NULL)
    {
        fprintf(stderr, "Error opening the assembly file.\n");
        return;
    }

    parseFileHandleSymbols(assembly_file, symbol_table, extern_table, passed_first);

    fclose(assembly_file);
}