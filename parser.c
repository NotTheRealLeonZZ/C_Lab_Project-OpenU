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
#include "symbol.h"
#include "instructions.h"
#include "directives.h"
#include "globals.h"

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

/* A method to check for comma problems in "mcro" declaration
Assuming there are no commas in macro decleration  */
bool commaInLine(char *input)
{
    int i = 0;

    cleanAllSpaces(input);

    /* Check for commas */
    while (input[i] != '\0')
    {
        if (input[i] == ',')
        {
            fprintf(stdout, "Wrong placed comma in macro decleration.\n");
            return false;
        }
        i++;
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

int storeWords(char *line, char words[][MAX_LINE_LENGTH], int num_words)
{
    char *token = strtok((char *)line, " ,");

    while (token != NULL && num_words < MAX_LINE_LENGTH)
    {
        strcpy(words[num_words++], token);
        token = strtok(NULL, " ,");
    }

    return num_words;
}

/*

The main parsing function for assembly files.
This file will take care of storing macros in macros table and replacing them in the original file
Later on, it will also make all the parsing for the file, including validating ALL lines.

*/
bool parseFileHandleMacros(FILE *assembly_file, FILE *am_file, char *am_file_name, struct Macro *macro_table_head)
{
    char line[MAX_LINE_LENGTH];      /* Variable to hold the current line */
    char line_copy[MAX_LINE_LENGTH]; /* A copy of the line, to manipulate without losing the original line */
    bool is_inside_macro = false;    /* Flag to check if the words im reading are part of a macro */
    bool create_AM = true;
    char current_macro_name[MAX_LINE_LENGTH - 5]; /* Variable to hold current macro name */
    char current_macro_data[MAX_LINE_LENGTH];     /* Variable to hold current macro data (command line) */
    struct Macro *macro_table_head_copy;          /* A copy of the macro table head node, to manipulate without losing the original pointer */
    struct Macro *new_macro;                      /* New macro to add to the macro table */
    char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH]; /* 2 dim array to hold all the parsed words from a line*/
    int num_words = 0;                            /* Counter for words captured from line */
    int line_number = 1;                          /* Counter for lines in file */

    /* Read a valid string and feed it into "line" variable and checks if it was successful*/
    while (fgets(line, MAX_LINE_LENGTH, assembly_file) != NULL)
    {
        /* Reset pointer to copy */
        macro_table_head_copy = macro_table_head;
        new_macro = NULL;

        strcpy(line_copy, line);

        removeNewLineFromEnd(line_copy); /* Remove new line from end of input */

        /* Parse the line into words and count. */
        num_words = storeWords(line_copy, words, num_words);

        /* Assuming in macro declaration, first word is mcro */
        if (strcmp(words[0], "mcro") == 0)
        {
            if (!commaInLine(line_copy))
            {
                cleanLeadingSpaces(line_copy); /* Remove spaces before first word */

                if (num_words != 2)
                {
                    fprintf(stdout, "Error: Malformed macro declaration in line %d\n", line_number);
                    create_AM = false;
                }
                else
                {
                    /* Correct number of words in macro decleration
                    Now validate macro's name */
                    if (isInstructionName(words[1]) || isDirectiveName(words[1]))
                    {
                        fprintf(stdout, "Error: illegal macro name in line %d\n", line_number);
                        create_AM = false;
                    }
                    else
                    {
                        /* Valid macro opening, flag it */
                        is_inside_macro = true;

                        /* words[1] has the macro name */
                        strcpy(current_macro_name, words[1]);

                        /* Dont copy this line to am file */
                    }
                }
            }
        }

        /* Assuming in macro end declaration, first word is endmcro */

        else if (strcmp(words[0], "endmcro") == 0)
        {
            if (!commaInLine(line_copy))
            {
                cleanLeadingSpaces(line_copy); /* Remove spaces before first word */

                if (num_words != 1)
                {
                    fprintf(stdout, "Error: Malformed macro end declaration in line %d\n", line_number);
                    create_AM = false;
                }
                else
                {
                    /* Finished to declear macro, flag it */
                    is_inside_macro = false;
                    /* Dont copy to .am file */
                }
            }
        }
        else
        {
            /* First word in unknown at the moment,
            Check if we are inside a macro decleration, save it.
            If not, check if its a known macro to replace it. */

            if (is_inside_macro)
            {

                /* Reset line_copy to current line */
                strcpy(line_copy, line);

                /* Store the macro line into the data variable */
                strcpy(current_macro_data, line_copy);

                /* Create a new macro node */
                new_macro = createMacro(current_macro_name, current_macro_data);

                /* Add this node to the end of the macro table */
                addMacro(macro_table_head_copy, new_macro);

                /* Dont copy to .am file */
            }
            else
            {
                /* We are not inside a macro decleration.
                That means this line is either a known macro or a unknown line.
                If its a known macro, we'll write it's data to .am file
                And if its an unknown line, just write it to .am file, we'll deal with it later */

                /* Reset line_copy to current line */
                strcpy(line_copy, line);

                struct Macro *searchedMacro = findMacro(macro_table_head_copy->next, words, num_words);

                if (searchedMacro != NULL)
                {
                    /* This line has a known macro, write the macro data to .am file*/
                    int i;
                    for (i = 0; i < num_words; i++)
                    {
                        if (strcmp(searchedMacro->name, words[i]) == 0)
                        {
                            writeMacroToOutput(searchedMacro, am_file);
                        }
                        else
                        {
                            if (i == num_words - 1)
                            {
                                fputs(words[i], am_file);
                                fputs("\n", am_file);
                            }
                            else
                            {
                                fputs(words[i], am_file);
                                fputs("  ", am_file);
                            }
                        }
                    }
                }
                else
                {
                    /* Check if this line has been written before */
                    /* Check if need to check */
                    if (!isEmptyOrCommentLines(line_copy))
                        fputs(line, am_file);
                }
            }
        }

        /* Reset words array */
        memset(words, '\0', sizeof(words));
        num_words = 0;

        line_number += 1;
    }

    if (!create_AM)
    {
        fclose(am_file);
        if (remove(am_file_name) != 0)
        {
            fprintf(stderr, "Error: Failed to delete .am file.\n");
        }
        return false;
    }
    return true;
}

void parseFileHandleSymbols(FILE *assembly_file, struct Symbol *symbol_table_head)
{
    char line[MAX_LINE_LENGTH];                       /* Variable to hold the current line */
    char line_copy[MAX_LINE_LENGTH];                  /* A copy of the line, to manipulate without losing the original line */
    char current_symbol_name[MAX_SYMBOL_NAME_LENGTH]; /* Variable to hold current symbol's name */
    int line_number = 0;                              /* Line number counter for error messages */
    int current_symbol_line;                          /* Variable to hold current symbol's line number */
    char current_symbol_data[3];                      /* Variable to hold current symbol's type (ins or dir) */
    struct Symbol *symbol_table_head_copy;            /* A copy of the macro table head node, to manipulate without losing the original pointer */
    struct Symbol *new_symbol;                        /* New symbol to add to the symbol table */
    char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH];     /* 2 dim array to hold all the parsed words from a line*/
    int num_words = 0;                                /* Counter for words captured from line */

    while (fgets(line, MAX_LINE_LENGTH, assembly_file) != NULL)
    {
        /* Reset pointer to copy */
        symbol_table_head_copy = symbol_table_head;

        /* Make a copy of line to work on */
        strcpy(line_copy, line);

        removeNewLineFromEnd(line_copy); /* Remove new line from end of input */

        /* Parse the line into words and count. */
        num_words = storeWords(line_copy, words, num_words);

        /* Checks if current line has a valid symbol */

        if (wordIsSymbol(symbol_table_head_copy, words[0]))
        {
            /* Valid name of symbol, check syntax */
            printf("Found a symbol! \"%s\"\n", words[0]);
        }

        /* int i;
        for (i = 0; i < num_words; i++)
        {
            printf("%s ->", words[i]);
        }
        printf("\n"); */

        /* Reset words array */
        memset(words, '\0', sizeof(words));
        num_words = 0;

        line_number += 1;
    }
}
