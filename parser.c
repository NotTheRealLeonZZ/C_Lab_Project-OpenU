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
#include <ctype.h>
#include "parser.h"
#include "macro.h"
#include "instructions.h"

void cleanLeadingSpaces(char *input)
{
    int i, j = 0;

    /* Find the first non-space and non-tab character */
    for (i = 0; input[i] != '\0'; i++)
    {
        if (!isspace(input[i]))
        {
            break;
        }
    }

    /* Move the remaining characters to the beginning of the string */
    while (input[i] != '\0')
    {
        input[j++] = input[i++];
    }

    /* Add the null terminator at the end of the new string */
    input[j] = '\0';
}

/* A method to remove spaces before and after input */
void cleanAllSpaces(char *input)
{
    char *src = input;
    char *dst = input;

    while (*src != '\0')
    {
        if (!isspace((unsigned char)*src))
        {
            *dst++ = *src;
        }
        src++;
    }

    *dst = '\0';
}

/* A method to check for comma problems like too many or not enough */
bool commaProblem(char *input)
{
    int i = 0;
    int count = 0;

    cleanAllSpaces(input);

    /* Count commas */
    while (input[i] != '\0')
    {
        if (input[i] == ',' && input[i + 1] == ',') /* If there are 2 commas in a row */
        {
            fprintf(stderr, "Multiple consecutive commas\n");
            return true;
        }
        if (input[i] == ',')
        {
            count = count + 1;
        }
        i++;
    }

    /* Check if the input has instructions, and checks comma count */
    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {
        if (strstr(input, instructionsArray[i].name) != NULL)
        {
            if (instructionsArray[i].operands_num > 0)
            {
                if (count != (instructionsArray[i].operands_num - 1))
                {
                    /* printf("number of commas should be is: %d\n", instructionsArray[i].operands_num - 1); */
                    fprintf(stderr, "ERROR: Incorrect number of commas for instruction: %s\n", instructionsArray[i].name);
                    return true;
                }
            }
            else
            {
                /* printf("number of commas should be is: 0\n"); */
                if (count != 0)
                {
                    fprintf(stderr, "ERROR: Incorrect number of commas for instruction: %s\n", instructionsArray[i].name);
                    return true;
                }
            }
        }
    }

    return false; /* No problems */
}

void removeNewLineFromEnd(char *line)
{

    line[strcspn(line, "\n")] = '\0'; /* Remove new line from end of input */
}

bool isEmptyOrCommentLines(char *line)
{

    /* Skip empty lines or lines starting with ';' */
    if (line[0] == '\0' || line[0] == ';')
    {
        return true;
    }

    return false;
}

int storeWords(char *line, char words[][MAX_LINE_LENGTH], int numWords)
{
    char *token = strtok((char *)line, " ,");

    while (token != NULL && numWords < 50)
    {
        strcpy(words[numWords++], token);
        token = strtok(NULL, " ,");
    }

    return numWords;
}

/*

The main parsing function for assembly files.
This file will take care of storing macros in macros table and replacing them in the original file
Later on, it will also make all the parsing for the file, including validating ALL lines.

*/
void parseFileHandleMacros(FILE *assembly_file, FILE *am_file, char *am_file_name, struct Macro *macro_table_head)
{
    char line[MAX_LINE_LENGTH];                   /* Variable to hold the current line */
    char line_copy[MAX_LINE_LENGTH];              /* A copy of the line, to manipulate without losing the original line */
    bool is_inside_macro = false;                 /* Flag to check if the words im reading are part of a macro */
    bool line_written = false;                    /* Did I write this line already? */
    char current_macro_name[MAX_LINE_LENGTH - 5]; /* Variable to hold current macro name */
    char current_macro_data[MAX_LINE_LENGTH];     /* Variable to hold current macro data (command line) */
    struct Macro *macro_table_head_copy;          /* A copy of the macro table head node, to manipulate without losing the original pointer */
    struct Macro *new_macro;
    char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int numWords = 0;

    /* Read a valid string and feed it into "line" variable and checks if it was successful*/
    while (fgets(line, MAX_LINE_LENGTH, assembly_file) != NULL)
    {
        /* Reset pointer to copy */
        macro_table_head_copy = macro_table_head;
        line_written = false;

        strcpy(line_copy, line);

        /* Make adjustments for macro comma check */
        if (!commaProblem(line_copy))
        {
            /* No commas problem */
            strcpy(line_copy, line);

            removeNewLineFromEnd(line_copy); /* Remove new line from end of input */

            if (!isEmptyOrCommentLines(line_copy))
            {

                cleanLeadingSpaces(line_copy);

                numWords = storeWords(line_copy, words, numWords);

                /* Check if first word is 'mcro' */

                if (strcmp(words[0], "mcro") == 0)
                {

                    if (numWords != 2)
                    {
                        fprintf(stderr, "Error: Malformed macro declaration.{add line}\n");
                        /* MAKE SURE TO DELETE AM FILE AT THE END */
                        /* amFile = false */;
                    }
                    else
                    {
                        /* Validate macro name */
                        if (isInstructionName(words[1]))
                        {
                            fprintf(stderr, "Error: Macro name cannot be same as instruction name.\n");
                            /* MAKE SURE TO DELETE AM FILE AT THE END */

                            /* NOT WORKING BECAUSE WHEN MACRO NAME IS INSTRUCTION IT HAS COMMA PROBLEM */
                            fclose(am_file);
                            if (remove(am_file_name) != 0)
                            {
                                fprintf(stderr, "Error: Failed to delete .am file.\n");
                            }

                        }
                        else
                        {

                            /* We are inside a macro declaration */
                            is_inside_macro = true;

                            /* words[1] has the macro name */
                            strcpy(current_macro_name, words[1]);

                            /* Dont copy to .am file */
                        }
                    }
                }

                else if (strcmp(words[0], "endmcro") == 0)
                {
                    /* Finished to declear macro */
                    is_inside_macro = false;
                    /* Dont copy to .am file */
                }
                else
                {
                    /* Search for the macro in our Macro table, and return the pointer */
                    struct Macro *searchedMacro = findMacro(macro_table_head_copy, words[0]);

                    if (searchedMacro != NULL)
                    {
                        /* Write macro to .am file as long as name appears in the macro list*/
                        writeMacroToOutput(searchedMacro, am_file);
                        line_written = true;
                    }

                    /* ADD SITUATION WHEN WE COME ACROSS WORD THAT IS NOT KNOWN AND NOT A MACRO */



                    /* Checking if in macro so we can copy next line (syntax doesnt matter) */
                    if (is_inside_macro)
                    {
                        /* Reset pointers */
                        macro_table_head_copy = macro_table_head;
                        new_macro = NULL;

                        /* Reset line read */
                        strcpy(line_copy, line);

                        /* Store the macro line into the data variable */
                        strcpy(current_macro_data, line_copy);

                        /* Create a new macro node */
                        new_macro = createMacro(current_macro_name, current_macro_data);

                        /* Add this node to the end of the macro table */
                        addMacro(macro_table_head_copy, new_macro);

                        /* Dont copy to .am file */
                    }
                    else if (!line_written)
                    {
                        fputs(line, am_file);
                    }
                }
            }
        }
        else
        {
            /* amFile = false */;
            /* Comma problem */
        }

        /* int i;
        printf("Words in the array: ");
        for (i = 0; i < numWords; i++)
        {
            printf("\'%s\' -> ", words[i]);
        }
        printf("\n"); */

        memset(words, '\0', sizeof(words));
        numWords = 0;
    }

    /* if there was at least 1 macro, run function to output .am file with the macro list (This about moving this as soon as macro was found) */
}

void parseFileSetMacro(FILE *assembly_file, struct Macro *macro_table_head)
{
}
