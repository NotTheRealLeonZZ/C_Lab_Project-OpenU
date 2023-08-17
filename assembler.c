#include <stdio.h>
#include "pre_assembler.h"
#include "parser.h"
#include "first_pass.h"
#include "second_pass.h"
#include "globals.h"
#include "symbol.h"
#include "extern.h"
#include "binary.h"
#include "tables.h"
#include "variables.h"
#include "encoding.h"

/* The actual assembler, running on each agrument the user entered */
void runAssembler(char *argv[], int index)
{
    char as_filename[MAX_FILE_NAME_LENGTH];
    char am_filename[MAX_FILE_NAME_LENGTH];
    char ob_filename[MAX_FILE_NAME_LENGTH];
    char ent_filename[MAX_FILE_NAME_LENGTH];
    char ext_filename[MAX_FILE_NAME_LENGTH];
    int passed_first = 1;  /* Passed first? */
    int passed_second = 1; /* Passed second? */
    int is_ent = 0;        /* Variable to check if there is .entry command in code */
    int is_ext = 0;        /* Variable to check if there is .extern command in code */
    FILE *ob_file;         /* Object file */
    FILE *ent_file;        /* Entry file */
    FILE *ext_file;        /* Extern file */
    int ic = 0;            /* Instructions counter */
    int dc = 0;            /* Directives counter */

    /* Create the symbol table */
    struct Symbol *symbol_table = createSymbol("Symbol_table", -1, "Symbol_type");

    /* Create the extern table */
    struct Extern *extern_table = createExtern("Extern_table");

    /* Create the binary code table */
    struct Binary *binary_code_table = createBinary("Binary_code_table", "Binary_type");

    /* Create the variables table */
    struct Variable *variable_table = createVariable("Variable_table", -1, "Variable_type");

    /* Extract the filename from the command-line argument  */

    sprintf(as_filename, "%.*s.as", (int)sizeof(as_filename) - 4, argv[index]);
    sprintf(am_filename, "%.*s.am", (int)sizeof(am_filename) - 4, argv[index]);
    sprintf(ob_filename, "%.*s.ob", (int)sizeof(ob_filename) - 4, argv[index]);
    sprintf(ent_filename, "%.*s.ent", (int)sizeof(ent_filename) - 4, argv[index]);
    sprintf(ext_filename, "%.*s.ext", (int)sizeof(ext_filename) - 4, argv[index]);

    /* Spread macros over .am file */
    preAssemble(as_filename, am_filename);

    /* First pass - Create tables */
    firstPass(am_filename, symbol_table, extern_table, &passed_first, &is_ent);
    if (passed_first)
    {
        /* All lines has correct syntax,
        Now start building the binary codes for each line. */

        /* Check that extern and symbol(entry) are not the same because entry cannot be extern and vice versa */
        if (tablesAreDifferent(symbol_table, extern_table))
        {
            secondPass(am_filename, symbol_table, extern_table, binary_code_table, variable_table, &passed_second, &ic, &dc, &is_ext);
            if (passed_second)
            {

                /* Create entry and extern files */
                if (is_ent)
                {
                    ent_file = fopen(ent_filename, "w");
                    if (ent_file == NULL)
                    {
                        fprintf(stderr, "Error opening the assembly file.\n");
                    }
                    writeEntVariablesToFile(ent_file, variable_table);
                    fclose(ent_file);
                }

                if (is_ext)
                {
                    ext_file = fopen(ext_filename, "w");
                    if (ext_file == NULL)
                    {
                        fprintf(stderr, "Error opening the assembly file.\n");
                    }
                    writeExtVariablesToFile(ext_file, variable_table);

                    fclose(ext_file);
                }

                /* Create object file */
                ob_file = fopen(ob_filename, "w");
                if (ob_file == NULL)
                {
                    fprintf(stderr, "Error opening the assembly file.\n");
                }
                writeEncodedProgramToFile(ob_file, binary_code_table, &ic, &dc);
                fclose(ob_file);
            }
            else
            {
                /* Second pass wasn't successful */
                if (remove(am_filename) != 0)
                {
                    fprintf(stderr, "Error: Failed to delete .am file.\n");
                }
            }
        }
        else
        {
            /* Extern table and symbol table has same symbol, remove am file */
            if (remove(am_filename) != 0)
            {
                fprintf(stderr, "Error: Failed to delete .am file.\n");
            }
        }
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
    freeBinaryTable(&binary_code_table);
    freeVariableTable(&variable_table);
}

int main(int argc, char *argv[])
{
    int i;

    /* Validate command line 
    Get the file name from the input */
    if (argc < 2) /* MAYBE CHECK IF ALL FILE NAMES HAS .AS FILE IN THIS FOLDER */
    {
        fprintf(stderr, "Usage: %s <filename_without_extension>\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++)
    {
        runAssembler(argv, i);
    }

    return 0;
}
