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
#include "memory.h"

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

void removeTrailingSpaces(char *input)
{
    int length = strlen(input);
    int i = length - 1;

    /* Find the index of the last non-space character */
    while (i >= 0 && input[i] == ' ')
    {
        i--;
    }

    /* Replace trailing spaces with the null terminator */
    input[i + 1] = '\0';
}

/* A method to remove spaces entirely from input */
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

/* A method to check for a comma in line.
Assuming there are no commas in macro/extern/entry and instruction with no parameters decleration  */
bool commaInLine(char *input)
{
    int i = 0;

    cleanAllSpaces(input);

    /* Check for commas */
    while (input[i] != '\0')
    {
        if (input[i] == ',')
            return true;
        i++;
    }
    return false; /* No problems */
}

bool commaAtFirstOrLast(char *input)
{
    char input_copy[MAX_LINE_LENGTH];
    strcpy(input_copy, input);
    size_t length = strlen(input_copy);

    if (*input_copy == ',' || input_copy[length - 1] == ',')
        return true; /* first character is a comma */
    return false;
}

bool quoteAtFirstAndLast(char *input)
{
    char input_copy[MAX_LINE_LENGTH];
    strcpy(input_copy, input);
    size_t length = strlen(input_copy);
    if ((*input_copy == '"' || *input_copy == ' ') && (input_copy[length - 1] == '"' || input_copy[length - 1] == ' '))
        return true; /* first and last characters are double quotes */
    return false;
}

/* add description
@return true if there is a comma after the first word.
Also, modifies the line to return without the first word for later parameter validation */
bool commaAfterFirstWord(char *line, char *first_word)
{
    char new_line[MAX_LINE_LENGTH];
    int index;

    if (line != NULL)
    {
        /* Create new line without the first word */
        index = strlen(first_word);
        strcpy(new_line, line + index);
    }

    cleanAllSpaces(new_line);
    if (commaAtFirstOrLast(new_line))
        return true;

    /* Copy modified line to parse for parameters easily */
    strcpy(line, new_line);
    return false;
}

bool doubleComma(char *line, int line_number)
{
    int i = 0;
    char line_copy[MAX_LINE_LENGTH];

    strcpy(line_copy, line);
    cleanAllSpaces(line_copy);

    while (line_copy[i] != '\0')
    {
        if (line_copy[i] == ',' && line_copy[i + 1] == ',') /* If there are 2 commas in a row */
        {
            fprintf(stdout, "Error! in line %d: Multiple consecutive commas\n", line_number);
            return true;
        }
        i++;
    }
    return false;
}

int countCommas(char *line)
{
    int comma_count = 0;
    int i = 0;
    char line_copy[MAX_LINE_LENGTH];

    strcpy(line_copy, line);
    cleanAllSpaces(line_copy);

    while (line_copy[i] != '\0')
    {
        if (line_copy[i] == ',')
        {
            comma_count += 1;
        }
        i++;
    }
    return comma_count;
}

int countQuotes(char *line)
{
    int quote_count = 0;
    int i = 0;
    char line_copy[MAX_LINE_LENGTH];

    strcpy(line_copy, line);
    cleanAllSpaces(line_copy);

    while (line_copy[i] != '\0')
    {
        if (line_copy[i] == '"')
        {
            quote_count += 1;
        }
        i++;
    }

    return quote_count;
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

int tokenStrings(char *line, char words[][MAX_LINE_LENGTH], int num_words)
{
    num_words = 0;
    char *token;

    /* Copy the input line to a temporary buffer since strtok modifies the original string */
    char line_copy[MAX_LINE_LENGTH];
    strncpy(line_copy, line, sizeof(line_copy));
    line_copy[sizeof(line_copy) - 1] = '\0'; /* Ensure null-termination */

    /* Extract the first word (e.g., ".string") */
    token = strtok(line_copy, " ");
    if (token != NULL)
    {
        strncpy(words[num_words++], token, MAX_LINE_LENGTH - 1);
    }
    else
    {
        return num_words; /* Return 0 if no first word found */
    }

    /* Extract the content between the double quotes */
    token = strtok(NULL, "\"");
    if (token != NULL && num_words < MAX_LINE_LENGTH)
    {
        strncpy(words[num_words++], token, MAX_LINE_LENGTH - 1);
    }

    return num_words;
}

void resetLineCopy(char *line, char *line_copy)
{
    strcpy(line_copy, line);
    cleanLeadingSpaces(line_copy);
    removeNewLineFromEnd(line_copy);
}

bool isInteger(char *param, int line_number)
{
    char *endptr; /* Pointer to the first non-long int character */

    strtol(param, &endptr, 10);

    if (*endptr == '\0')
        return true;
    else
    {
        fprintf(stdout, "Error! in line %d: Parameter %s should have been an integer and it's not.\n", line_number, param);
        return false;
    }
}

bool isAscii(char *line, int line_number)
{
    while (*line != '\0')
    {
        /*  Check if the ASCII code of the character is within the valid range  */
        if ((unsigned char)(*line) > 127)
        {
            return false;
        }
        line++;
    }
    return true;
}

void removeSubString(char *line, char *sub_string)
{
    int index = strlen(sub_string);
    strcpy(line, line + index);
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
    char current_macro_name[MAX_MACRO_NAME];      /* Variable to hold current macro name */
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

        resetLineCopy(line, line_copy);

        /* Parse the line into words and count. */
        num_words = storeWords(line_copy, words, num_words);

        /* Assuming in macro declaration, first word is mcro */
        if (strcmp(words[0], "mcro") == 0)
        {
            if (!commaInLine(line_copy))
            {

                if (num_words != 2)
                {
                    fprintf(stdout, "Error! in line %d Malformed macro declaration.\n", line_number);
                    create_AM = false;
                }
                else
                {
                    /* Correct number of words in macro decleration
                    Now validate macro's name */
                    if (isInstructionName(words[1]) || isDirectiveName(words[1]))
                    {
                        fprintf(stdout, "Error! in line %d: illegal macro name.\n", line_number);
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
                fprintf(stdout, "Error! in line %d: unnecessary comma.\n", line_number);
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
                    fprintf(stdout, "Error! in line %d: Malformed macro end declaration.\n", line_number);
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
    int line_number = 1;                              /* Line number counter for error messages */
    int memory_count = MEMORY_START;                  /* A counter for memory location, to address symbols */
    char current_symbol_name[MAX_SYMBOL_NAME_LENGTH]; /* Variable to hold current symbol's name */
    int current_symbol_address;                       /* Variable to hold current symbol's address number */
    char current_symbol_type[SYMBOL_TYPE_LENGTH];     /* Variable to hold current symbol's type (ins or dir) */
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
        cleanLeadingSpaces(line_copy);   /* Clear leading spaces of line */
        removeNewLineFromEnd(line_copy); /* Remove new line from end of input */

        /* Parse the line into words and count. */
        num_words = storeWords(line_copy, words, num_words);

        if (!commaAtFirstOrLast(line_copy)) /* Check if a comma is presented at start or finish */
        {
            /* Checks if current line has a valid symbol and remove ':' from the end */
            if (wordIsSymbol(symbol_table_head_copy, words[0]))
            {
                /* Valid name of symbol, check syntax */
                strcpy(current_symbol_name, words[0]);
                printf("Found a symbol! \"%s\"\n", current_symbol_name);

                /* line_copy holds the name of the symbol including the ':',
            make a copy of the original line without the symbol */

                removeSymbolFromLine(line, line_copy);
                cleanLeadingSpaces(line_copy);
                removeNewLineFromEnd(line_copy);

                printf("modified line_copy: %s\n", line_copy);
                /* line_copy holds the data of the symbol, either a directive or instruction
            copy it to original line */
                strcpy(line, line_copy);

                /* Check for comma as first word or last character */
                if (!commaAtFirstOrLast(line_copy))
                {
                    /* Check the type of first word, and validate */
                    /* Parse just the data of the symbol, without the symbol name, to words array. */

                    /* Reset words array */
                    memset(words, '\0', sizeof(words));
                    num_words = 0;
                    num_words = storeWords(line_copy, words, num_words);

                    resetLineCopy(line, line_copy);

                    if (isDirectiveName(words[0]))
                    {

                        /* Validate directive syntax first pass */
                        if (validDirective(words, num_words, line_copy, line_number))
                        {
                            warnSymbolIfNecessary(words[0], line_number);
                            /* No need to create symbol for extern and entry.
                            Validation is enough, later we'll deal with it */
                            if (strcmp(words[0], ".extern") != 0 && strcmp(words[0], ".entry") != 0)
                            {
                                /* Valid syntax of directive,
                                type: dir
                                address: memory_count */
                                current_symbol_address = memory_count;
                                strcpy(current_symbol_type, "dir");

                                new_symbol = createSymbol(current_symbol_name, current_symbol_address, current_symbol_type);
                                addSymbol(symbol_table_head_copy, new_symbol);

                                printf("preform memory promotion to %s\n", line_copy);
                                memory_count = promoteMemory(memory_count, line_copy, num_words, words[0]);
                            }
                        }
                    }

                    else if (isInstructionName(words[0]))
                    {
                        printf("This symbol contains an instruction: %s\n", words[0]);
                        /* Validate instruction syntax for first pass */
                        if (validInstruction(words, num_words, line_copy, line_number))
                        {
                        }
                    }
                }
                else
                {
                    fprintf(stdout, "Error! in line %d: Invalid comma at beginning/end of line.\n", line_number);
                    /* Flag for assembler not to continue to second pass and dont output any files (delete am) */
                }
            }
        }
        else
        {
            fprintf(stdout, "Error! in line %d: Invalid comma at beginning/end of line.\n", line_number);
        }

        /* Not a symbol, the only thing that interesting here is the line count,
        because each command can add lines depends on the addressing method. */

        int i;
        for (i = 0; i < num_words; i++)
        {
            printf("%s -> ", words[i]);
        }
        printf("\n");
        printf("===================\n");

        /* Reset words array */
        memset(words, '\0', sizeof(words));
        num_words = 0;

        line_number += 1;
        memory_count += 1;
    }
}
