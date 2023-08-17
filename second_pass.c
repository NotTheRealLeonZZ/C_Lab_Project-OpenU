#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "parser.h"
#include "extern.h"
#include "binary.h"
#include "variables.h"

/* Function to perform the second pass tasks */
void secondPass(const char *input_file, struct Symbol *symbol_table, struct Extern *extern_table, struct Binary *binary_code_table, struct Variable *variable_table, int *passed_second, int *ic, int *dc, int *is_ext)
{

    /* Openes the input file for reading and checks for errors when opening the file */
    FILE *assembly_file = fopen(input_file, "r");
    if (assembly_file == NULL)
    {
        fprintf(stderr, "Error opening the assembly file.\n");
        *passed_second = 0;
        return;
    }

    /* Parse 2nd pass */
    parseSecondPass(assembly_file, symbol_table, extern_table, binary_code_table, variable_table, passed_second, ic, dc, is_ext);
    fclose(assembly_file);
}