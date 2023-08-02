#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "symbol.h"

void firstPass(const char *input_file)
{

    /* Openes the input file for reading and checks for errors when opening the file */
    FILE *assembly_file = fopen(input_file, "r");
    if (assembly_file == NULL)
    {
        fprintf(stderr, "Error opening the assembly file.\n");
        return;
    }

    /* Create the symbol table */
    struct Symbol *symbol_table = createSymbol("Symbol_table", -100, "Symbol_type");

    parseFileHandleSymbols(assembly_file, symbol_table);

    printSymbolTable(symbol_table);

    freeSymbolTable(&symbol_table);
}