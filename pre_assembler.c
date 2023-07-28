#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pre_assembler.h"
#include "parser.h"
#include "macro.h"


/* 

    Pre assembler function.
    The main mission here is to replace all the macros in the original input file (.as)
    and make the file ready for the "First Assembler Pass" by outputing a new file (.am)
    @param input_file 

*/
void preAssemble(const char* input_file) {

    /* Openes the input file for reading and checks for errors in opening the file */
    FILE* assembly_file = fopen(input_file, "r");
    if (assembly_file == NULL) {
        fprintf(stderr, "Error opening the assembly file.\n");
        return;
    }

    /* Creates the macro table */
    struct Macro *macro_table = createMacro("Macro_table", "Starting of macro table");

    /* Parsing the original file to find all the macros and build the macro table. */
    parseFileGetMacro(assembly_file, macro_table);

    /* Close and reopen the file to start from the beginning */
    fclose(assembly_file);


    printMacroTable(macro_table);
    /* Test until here! */
    /*

    assembly_file = fopen(input_file, "r");
    if (assembly_file == NULL) {
        fprintf(stderr, "Error reopening the assembly file.\n");
        freeMacroTable(macro_table);
        return;
    }

    
    // Print the macro table



    



    // Second pass: Replace macros with their actual content
     
    FILE* outputFile = fopen("x.am", "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error creating the output file.\n");
        freeMacroTable(&macro_table);
        fclose(assembly_file);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, assembly_file) != NULL) {
        // Remove '\n' from the end of the line (if present) 
        line[strcspn(line, "\n")] = '\0';

        // Check if the line contains a macro name
        if (line[0] == 'm' && line[1] == 'c' && line[2] == 'r' && line[3] == 'o') {
            // Find the macro in the macro table
            char macroName[MAX_LINE_LENGTH];
            sscanf(line, "mcro %s", macroName);
            Macro* macro = findMacro(&macro_table, macroName);
            if (macro != NULL) {
                // Replace the macro name with its content
                fprintf(outputFile, "%s\n", macro->content);
            } else {
                fprintf(stderr, "Error: Undefined macro '%s'.\n", macroName);
            }
        } else {
            // Copy non-macro lines as is to the output file
            fprintf(outputFile, "%s\n", line);
        }
    }

    // Close the files and free memory 
    fclose(assembly_file);
    fclose(outputFile);
    freeMacroTable(&macro_table)

    */
    
}
