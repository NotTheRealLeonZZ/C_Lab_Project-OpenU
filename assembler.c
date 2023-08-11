#include <stdio.h>
#include "pre_assembler.h"
#include "parser.h"
#include "first_pass.h"
#include "globals.h"
#include "symbol.h"
#include "extern.h"

int main(int argc, char *argv[])
{
    char as_filename[MAX_FILE_NAME_LENGTH];
    char am_filename[MAX_FILE_NAME_LENGTH];
    int passed_first = 1;

    /* Create the symbol table */
    struct Symbol *symbol_table = createSymbol("Symbol_table", -1, "Symbol_type");

    /* Create the extern table */
    struct Extern *extern_table = createExtern("Extern_table");

    /* Validate command line 
    Get the file name from the input */
    if (argc != 2) /* CHANGE IT LATER TO FIT MY NEEDS - MAYBE CHECK IF ALL FILE NAMES HAS .AS FILE IN THIS FOLDER */
    {
        fprintf(stderr, "Usage: %s <filename_without_extension>\n", argv[0]);
        return 1;
    }

    /* Extract the filename from the command-line argument - DO IT IN A LOOP FOR ALL ARGV */

    snprintf(as_filename, sizeof(as_filename), "%s.as", argv[1]);

    snprintf(am_filename, sizeof(am_filename), "%s.am", argv[1]);

    printf("as file name: %s\tam file name: %s\n", as_filename, am_filename);

    /* Spread macros over .am file */
    preAssemble(as_filename, am_filename);

    /* First pass - Create tables */
    firstPass(am_filename, symbol_table, extern_table, &passed_first);
    if (passed_first)
    {
        /* All lines has correct syntax,
        Need to start building the binary codes for each line. */
        printSymbolTable(symbol_table);
        printExternTable(extern_table);
    }
    else
    {
        /* First pass wasn't successful, remove am file */
        if (remove(am_filename) != 0)
        {
            fprintf(stderr, "Error: Failed to delete .am file.\n");
        }
    }

    freeSymbolTable(&symbol_table);
    freeExternTable(&extern_table);

    return 0;
}
