#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pre_assembler.h"
#include "parser.h"
#include "macro.h"

/* 

    Pre assembler function.
    The main mission here is to replace all the macros in the original input file (.as)
    and make the file ready for the "First Assembler Pass" by outputing a new file (.am) with all the macros spread.
    @param input_file the .as file name
    @param output_file the .am file name 

*/
void preAssemble(const char *input_file, char *output_file)
{
    bool parsedMacros = true;
    FILE *am_file;
    FILE *assembly_file;
    struct Macro *macro_table;

    /* Openes the input file for reading and checks for errors when opening the file */
    assembly_file = fopen(input_file, "r");
    if (assembly_file == NULL)
    {
        fprintf(stderr, "Error opening the assembly file.\n");
        return;
    }

    /* Openes the am output file for writing and checks for errors when opening the file */
    am_file = fopen(output_file, "w");
    if (am_file == NULL)
    {
        fprintf(stderr, "Error opening the assembly file.\n");
        return;
    }

    /* Creates the macro table */
    macro_table = createMacro("Macro_table", "Starting of macro table");

    /* Parsing the original file to find all the macros and build a macro table.
    While parsing, if a macro is detected and found on macro table, it will replace it
    with the macro data in the output file 
    Macros will always be decleared before the call for the macro.
    If I read a word that that isn't a macro, ill copy it to the .am file anyways,
    On first pass I will prompt an error for it.*/
    parsedMacros = parseFileHandleMacros(assembly_file, am_file, output_file, macro_table);

    if (parsedMacros)
    {
        /* Close the files */
        fclose(assembly_file);
        fclose(am_file);
    }
    else
    {
        /* Close only original file cause .am file was closed before deleteing */
        fclose(assembly_file);
    }

    /* free memory */
    printMacroTable(macro_table);

    freeMacroTable(&macro_table);
}
