/*

This file is going to parse the assembly file for pre-assembler.
Main focuse here is to find and store macros in the macro table
And replace all the macros with the actual macro.
After inputing all the macros, a file with .am extention will be outputed,
Ready for assembler "First pass"


 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "macro.h"

/*

The main parsing function for assembly files.
This file will take care of storing macros in macros table and replacing them in the original file
Later on, it will also make all the parsing for the file, including validating ALL lines.

*/
void parseFileGetMacro(FILE *assembly_file, struct Macro *macro_table_head)
{
    char line[MAX_LINE_LENGTH];                   /* Variable to hold the current line */
    char line_copy[MAX_LINE_LENGTH];              /* A copy of the line, to manipulate without losing the original line */
    bool is_inside_macro = false;                 /* Flag to check if the words im reading are part of a macro */
    bool macro_in_file = false;                   /* Is there at least 1 macro in file? */
    char current_macro_name[MAX_LINE_LENGTH - 5]; /* Variable to hold current macro name */
    char current_macro_data[MAX_LINE_LENGTH];     /* Variable to hold current macro data (command line) */
    struct Macro *macro_table_head_copy;          /* A copy of the macro table head node, to manipulate without losing the original pointer */
    struct Macro *new_macro;

    /* Read a valid string and feed it into "line" variable and checks if it was successful*/
    while (fgets(line, MAX_LINE_LENGTH, assembly_file) != NULL)
    {
        strcpy(line_copy, line);
        /* Remove '\n' from the end of the line (if present) */
        line_copy[strcspn(line_copy, "\n")] = '\0';

        /* Skip empty lines or lines starting with ';' */
        if (line_copy[0] == '\0' || line_copy[0] == ';')
        {
            continue;
        }

        /* Tokenize the line into words */
        char *word = strtok(line_copy, " ,"); /* Tokenize using space and comma as delimiters */
        char *instruction = word;             /* The first word is the instruction */


        /* #### Validate instruction name and params! ADD HERE instructions.c ##### */
        /* Check if the instruction is a start of a macro */
        if (strcmp(instruction, "mcro") == 0)
        {
            /* There is a macro in this file */
            macro_in_file = true;

            /* We are inside a macro declaration */
            is_inside_macro = true;

            /* Get the macro name */
            word = strtok(NULL, " ,");

            /* Validate macro name! ADD HERE*/

            if (word == NULL)
            {
                fprintf(stderr, "Error: Malformed macro declaration.\n");
                continue;
            }

            strcpy(current_macro_name, word);
        }

        else if (strcmp(instruction, "endmcro") == 0)
        {
            /* End of the current macro declaration */
            is_inside_macro = false;
        }
        else
        {
            /* Line should be a valid command, and NOT a START of macro */

            if (is_inside_macro)
            {
                /* Reset pointers */
                macro_table_head_copy = macro_table_head;
                new_macro = NULL;

                /* Reset line read */
                strcpy(line_copy, line);

                /* Remove '\n' from the end of the line (if present) */
                line_copy[strcspn(line_copy, "\n")] = '\0';

                /* Store the macro line into the data variable */
                strcpy(current_macro_data, line_copy);

                /* Create a new macro node */
                new_macro = createMacro(current_macro_name, current_macro_data);

                /* Add this node to the end of the macro table */
                addMacro(macro_table_head_copy, new_macro);
            }
            else
            {
                /* Perform syntax validation for non-macro lines */
                /* ... (your existing syntax validation code goes here) ... */
            }
        }
    }

    /* if there was at least 1 macro, run function to output .am file with the macro list (This about moving this as soon as macro was found) */
}
