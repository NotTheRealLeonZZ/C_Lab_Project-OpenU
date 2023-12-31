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
#include "extern.h"
#include "binary.h"
#include "variables.h"
#include "registers.h"

/* Remove leading spaces from a given string. */
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

/* Remove trailing spaces from a given string. */
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

/* Remove all spaces from a given string. */
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

/* Check if a comma is present in a given string.
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

/* Check if a comma is present at the first or last character of a string. */
bool commaAtFirstOrLast(char *input)
{
    size_t length;
    char input_copy[MAX_LINE_LENGTH];
    strcpy(input_copy, input);
    length = strlen(input_copy);

    if (*input_copy == ',' || input_copy[length - 1] == ',')
        return true; /* first character is a comma */
    return false;
}

/* Check if a string has double quotes at both the start and end. */
bool quoteAtFirstAndLast(char *input)
{
    size_t length;
    char input_copy[MAX_LINE_LENGTH];
    strcpy(input_copy, input);
    length = strlen(input_copy);
    if ((*input_copy == '"' || *input_copy == ' ') && (input_copy[length - 1] == '"' || input_copy[length - 1] == ' '))
        return true; /* first and last characters are double quotes */
    return false;
}

/* Check if a comma appears after the first word in a string.
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

/* Check if there are consecutive commas in a string. */
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

/* Count the number of commas in a string. */
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

/* Count the number of quotes in a string. */
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

/* Remove a newline character from the end of a string. */
void removeNewLineFromEnd(char *line)
{

    line[strcspn(line, "\n")] = '\0'; /* Remove new line from end of input */
}

/* Check if a line is empty or a comment line. */
bool isEmptyOrCommentLines(char *line)
{
    char line_copy[MAX_LINE_LENGTH];

    strcpy(line_copy, line);
    cleanAllSpaces(line_copy);

    /* Skip empty lines or lines starting with ';' */
    if (line_copy[0] == '\0' || line_copy[0] == ';' || line_copy[0] == '\n')
    {
        return true;
    }

    return false;
}

/* Tokenize a line into individual words. */
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

/* Tokenize a line into individual words, including handling special cases. */
int tokenStrings(char *line, char words[][MAX_LINE_LENGTH], int num_words)
{
    char line_copy[MAX_LINE_LENGTH];
    char *token;
    num_words = 0;

    /* Copy the input line to a temporary buffer since strtok modifies the original string */
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

/* Reset the copy of a line back to its original state. */
void resetLineCopy(char *line, char *line_copy)
{
    strcpy(line_copy, line);
    cleanLeadingSpaces(line_copy);
    removeNewLineFromEnd(line_copy);
}

/* Check if a string can be converted to an integer within a specified range. */
bool isIntegerInRange(char *param, int min_range, int max_range)
{
    char *endptr;
    long int value = strtol(param, &endptr, 10);

    if (*endptr == '\0' && value >= min_range && value <= max_range)
        return true;
    else
        return false;
}

/* Check if a string contains only ASCII characters. */
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

/* Remove a sub-string from a given string. */
void removeSubString(char *line, char *sub_string)
{
    int index = strlen(sub_string);
    strcpy(line, line + index);
}

/* Parse the assembly file, handling macros. */
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
    struct Macro *searchedMacro;                  /* Macro that will be found */
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
            resetLineCopy(line, line_copy);
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
            }
            else
            {
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

            if (is_inside_macro && !isEmptyOrCommentLines(line_copy))
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

                searchedMacro = findMacro(macro_table_head_copy, words, num_words);

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

/* Perform the first pass of the assembly process. */
void parseFirstPass(FILE *am_file, struct Symbol *symbol_table_head, struct Extern *extern_table_head, int *passed_first, int *is_ent)
{
    char line[MAX_LINE_LENGTH];                       /* Variable to hold the current line */
    char line_copy[MAX_LINE_LENGTH];                  /* A copy of the line, to manipulate without losing the original line */
    int line_number = 1;                              /* Line number counter for error messages */
    int memory_count = MEMORY_START;                  /* A counter for memory location, to address symbols */
    char current_symbol_name[MAX_SYMBOL_NAME_LENGTH]; /* Variable to hold current symbol's name */
    int current_symbol_address;                       /* Variable to hold current symbol's address number */
    char current_symbol_type[SYMBOL_TYPE_LENGTH];     /* Variable to hold current symbol's type (ins or dir) */
    struct Symbol *symbol_table_head_copy;            /* A copy of the symbol table head node, to manipulate without losing the original pointer */
    struct Symbol *new_symbol;                        /* New symbol to add to the symbol table */
    char current_extern_name[MAX_SYMBOL_NAME_LENGTH]; /* Variable to hold current extern's name */
    struct Extern *extern_table_head_copy;            /* A copy of the extern table head node, to manipulate without losing the original pointer */
    struct Extern *new_extern;                        /* New extern to add to the extern table */
    char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH];     /* 2 dim array to hold all the parsed words from a line*/
    int num_words = 0;                                /* Counter for words captured from line */
    int ic = 0;                                       /* Instruction counter */
    int dc = 0;                                       /* Directive counter */

    while (fgets(line, MAX_LINE_LENGTH, am_file) != NULL)
    {
        /* Reset pointer to copy */
        symbol_table_head_copy = symbol_table_head;

        /* Reset pointer to extern */
        extern_table_head_copy = extern_table_head;

        /* Make a copy of line to work on */
        strcpy(line_copy, line);
        cleanLeadingSpaces(line_copy);   /* Clear leading spaces of line */
        removeNewLineFromEnd(line_copy); /* Remove new line from end of input */

        /* Parse the line into words and count. */
        num_words = storeWords(line_copy, words, num_words);

        if (!commaAtFirstOrLast(line_copy)) /* Check if a comma is presented at start or finish */
        {
            /* Checks if current line has a valid symbol and remove ':' from the end */
            if (wordIsSymbol(words[0]))
            {
                if (!findSymbol(symbol_table_head_copy, words[0]))
                {

                    /* Valid name of symbol, check syntax */
                    strcpy(current_symbol_name, words[0]);

                    /* line_copy holds the name of the symbol including the ':',
                    make a copy of the original line without the symbol */

                    removeSymbolFromLine(line, line_copy);
                    cleanLeadingSpaces(line_copy);
                    removeNewLineFromEnd(line_copy);

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
                                    /* Valid syntax of directive, */

                                    current_symbol_address = MEMORY_START + dc;
                                    strcpy(current_symbol_type, "dir");
                                    new_symbol = createSymbol(current_symbol_name, current_symbol_address, current_symbol_type);
                                    addSymbol(symbol_table_head_copy, new_symbol);

                                    /* Count DC */
                                    if (strcmp(words[0], ".string") == 0)
                                    {
                                        removeTrailingSpaces(line_copy);
                                        dc += strlen(line_copy) - 2 + 1; /* removing 2 quotes and added '\0'  */
                                    }

                                    if (strcmp(words[0], ".data") == 0)
                                    {
                                        dc += num_words - 1;
                                    }

                                    memory_count = promoteMemoryDirective(memory_count, line_copy, num_words, words[0]);
                                    memory_count += 1;
                                }
                                else if (strcmp(words[0], ".extern") == 0)
                                {
                                    strcpy(current_extern_name, words[1]);
                                    new_extern = createExtern(current_extern_name);
                                    addExtern(extern_table_head_copy, new_extern);
                                }
                                else if (strcmp(words[0], ".entry") == 0)
                                {
                                    *is_ent = 1;
                                }
                            }
                            else
                            {
                                *passed_first = 0;
                            }
                        }

                        else if (isInstructionName(words[0]))
                        {
                            /* Validate instruction syntax for first pass */
                            if (validInstruction(words, num_words, line_copy, line_number))
                            {

                                current_symbol_address = MEMORY_START + ic;
                                strcpy(current_symbol_type, "ins");
                                new_symbol = createSymbol(current_symbol_name, current_symbol_address, current_symbol_type);
                                addSymbol(symbol_table_head_copy, new_symbol);

                                /* Count IC */
                                if (num_words == 3)
                                {
                                    if (isRegisterName(words[1]) && isRegisterName(words[2]))
                                        ic += num_words - 1;
                                    else
                                        ic += num_words;
                                }
                                else
                                {
                                    ic += num_words;
                                }

                                memory_count = promoteMemoryInstruction(memory_count, num_words, words);
                                memory_count += 1;
                            }
                            else
                            {

                                *passed_first = 0;
                            }
                        }
                        else
                        {
                            /* Symbol not holding instruction or directive */
                            fprintf(stdout, "Error! in line %d, Symbol has unknown instruction/directive.\n", line_number);
                            *passed_first = 0;
                        }
                    }
                    else
                    {
                        fprintf(stdout, "Error! in line %d: Invalid comma at beginning/end of line.\n", line_number);
                        *passed_first = 0;
                    }
                }
                else
                {
                    fprintf(stdout, "Error! in line %d, Double symbol decleration.\n", line_number);
                    *passed_first = 0;
                }
            }
            else
            {
                /* Not a symbol */
                resetLineCopy(line, line_copy);
                if (isDirectiveName(words[0]))
                {
                    /* Validate directive syntax first pass */
                    if (validDirective(words, num_words, line_copy, line_number))
                    {
                        if (strcmp(words[0], ".extern") != 0 && strcmp(words[0], ".entry") != 0)
                        {
                            /* Valid syntax of directive, */

                            /* Count DC */
                            if (strcmp(words[0], ".string") == 0)
                            {
                                removeTrailingSpaces(line_copy);
                                dc += strlen(line_copy) - 2 + 1; /* removing 2 quotes and added '\0'  */
                            }

                            if (strcmp(words[0], ".data") == 0)
                            {
                                dc += num_words - 1;
                            }

                            memory_count = promoteMemoryDirective(memory_count, line_copy, num_words, words[0]);
                            memory_count += 1;
                        }
                        else if (strcmp(words[0], ".extern") == 0)
                        {
                            strcpy(current_extern_name, words[1]);
                            new_extern = createExtern(current_extern_name);
                            addExtern(extern_table_head_copy, new_extern);
                        }
                        else if (strcmp(words[0], ".entry") == 0)
                        {
                            *is_ent = 1;
                        }
                    }
                    else
                    {
                        *passed_first = 0;
                    }
                }
                else if (isInstructionName(words[0]))
                {
                    /* Validate instruction syntax for first pass */
                    if (validInstruction(words, num_words, line_copy, line_number))
                    {
                        /* Count IC */
                        if (num_words == 3)
                        {
                            if (isRegisterName(words[1]) && isRegisterName(words[2]))
                                ic += num_words - 1;
                            else
                                ic += num_words;
                        }
                        else
                        {
                            ic += num_words;
                        }

                        memory_count = promoteMemoryInstruction(memory_count, num_words, words);
                        memory_count += 1;
                    }
                    else
                    {
                        *passed_first = 0;
                    }
                }
                else
                {
                    /* This line is not a symbol decleration/directive or instruction. */
                    fprintf(stdout, "Error! in line %d, Unknown instruction/directive.\n", line_number);
                    *passed_first = 0;
                }
            }
        }
        else
        {
            fprintf(stdout, "Error! in line %d: Invalid comma at beginning/end of line.\n", line_number);
            *passed_first = 0;
        }

        /* Not a symbol, the only thing that interesting here is the line count,
        because each command can add lines depends on the addressing method. */

        /* Reset words array */
        memset(words, '\0', sizeof(words));
        num_words = 0;

        line_number += 1;
    }

    memory_count -= 1; /* Fix last line */

    if (memory_count > MAX_PROGRAM_SIZE)
    {
        *passed_first = 0;
    }

    /* Fix address of symbols with complete IC */
    symbol_table_head_copy = symbol_table_head;
    fixSymbolAddress(symbol_table_head_copy, ic);
}

/* Perform the second pass of the assembly process. */
void parseSecondPass(FILE *am_file, struct Symbol *symbol_table_head, struct Extern *extern_table_head,
                     struct Binary *binary_code_table_head, struct Variable *variable_table_head, int *passed_second, int *ic, int *dc, int *is_ext)
{
    char line[MAX_LINE_LENGTH];                   /* Variable to hold the current line */
    char line_copy[MAX_LINE_LENGTH];              /* A copy of the line, to manipulate without losing the original line */
    int line_number = 1;                          /* Line number counter for error messages */
    int memory_count = MEMORY_START;              /* A counter for memory location, to address symbols */
    struct Symbol *symbol_table_head_copy;        /* A copy of the symbol table head node, to manipulate without losing the original pointer */
    struct Symbol *new_symbol;                    /* New symbol to add to the symbol table */
    struct Extern *extern_table_head_copy;        /* A copy of the extern table head node, to manipulate without losing the original pointer */
    struct Binary *binary_code_table_head_copy;   /* A copy of the binary_code table head node, to manipulate without losing the original pointer */
    struct Variable *variable_table_head_copy;    /* A copy of the variable table head node, to manipulate without losing the original pointer */
    struct Variable *new_variable;                /* New variable to add to the variable table */
    char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH]; /* 2 dim array to hold all the parsed words from a line*/
    int num_words = 0;                            /* Counter for words captured from line */

    while (fgets(line, MAX_LINE_LENGTH, am_file) != NULL)
    {

        /* Reset pointer to copy */
        symbol_table_head_copy = symbol_table_head;

        /* Reset pointer to extern */
        extern_table_head_copy = extern_table_head;

        /* Reset pointer to binary code */
        binary_code_table_head_copy = binary_code_table_head;

        /* Reset pointer to binary code */
        variable_table_head_copy = variable_table_head;

        /* Make a copy of line to work on */
        strcpy(line_copy, line);
        cleanLeadingSpaces(line_copy);   /* Clear leading spaces of line */
        removeNewLineFromEnd(line_copy); /* Remove new line from end of input */

        /* Parse the line into words and count. */
        num_words = storeWords(line_copy, words, num_words);

        if (wordIsSymbol(words[0]))
        {

            /* Its already in symbol table and correct */

            /* line_copy holds the name of the symbol including the ':',
            make a copy of the original line without the symbol */

            removeSymbolFromLine(line, line_copy);
            cleanLeadingSpaces(line_copy);
            removeNewLineFromEnd(line_copy);

            /* line_copy holds the data of the symbol, either a directive or instruction
            copy it to original line */
            strcpy(line, line_copy);

            /* Parse just the data of the symbol, without the symbol name, to words array. */

            /* Reset words array */
            memset(words, '\0', sizeof(words));
            num_words = 0;
            num_words = storeWords(line_copy, words, num_words);

            resetLineCopy(line, line_copy);

            if (isDirectiveName(words[0]))
            {
                /* Calculate dc increasing */
                if (strcmp(words[0], ".data") == 0)
                    *dc += num_words - 1;
                if (strcmp(words[0], ".string") == 0)
                {
                    /* Modify words to hold the entire string without quotes */
                    tokenStrings(line_copy, words, num_words);
                    *dc += strlen(words[1]) + 1; /* including '\0' */
                }
                /* Calculation for .data and .string */
                calculateDirectiveBinary(words, num_words, binary_code_table_head_copy);

                if (strcmp(words[0], ".entry") == 0)
                {
                    new_symbol = findSymbol(symbol_table_head, words[1]);
                    if (new_symbol == NULL)
                    {
                        fprintf(stdout, "Error! in line %d, Entry declaration but no symbol was found.\n", line_number);
                        *passed_second = 0;
                    }
                    else
                    {
                        new_variable = createVariable(new_symbol->name, new_symbol->address, "ent");
                        addVariable(variable_table_head_copy, new_variable);
                    }
                }

                if (strcmp(words[0], ".extern") != 0 && strcmp(words[0], ".entry") != 0)
                {
                    if (strcmp(words[0], ".string") == 0)
                    {
                        /* Remove .string from start of line_copy */
                        removeSubString(line_copy, words[0]);
                        cleanLeadingSpaces(line_copy);
                        memory_count = promoteMemoryDirective(memory_count, line_copy, num_words, words[0]);
                        memory_count += 1;
                    }
                    else
                    {
                        memory_count = promoteMemoryDirective(memory_count, line_copy, num_words, words[0]);
                        memory_count += 1;
                    }
                }
            }
            else if (isInstructionName(words[0]))
            {
                *ic += 1;
                calculateInstructionBinary(words, num_words, binary_code_table_head_copy, symbol_table_head_copy, variable_table_head_copy,
                                           extern_table_head_copy, &line_number, passed_second, &memory_count, ic, is_ext);
                memory_count += 1;
            }
        }
        else
        {
            /* Not a symbol */
            resetLineCopy(line, line_copy);

            if (isDirectiveName(words[0]))
            {
                /* Calculate dc increasing */
                if (strcmp(words[0], ".data") == 0)
                    *dc += num_words - 1;
                if (strcmp(words[0], ".string") == 0)
                {
                    /* Modify words to hold the entire string without quotes */
                    tokenStrings(line_copy, words, num_words);
                    *dc += strlen(words[1]);
                }
                /* Calculation for .data and .string */
                calculateDirectiveBinary(words, num_words, binary_code_table_head_copy);

                if (strcmp(words[0], ".entry") == 0)
                {
                    new_symbol = findSymbol(symbol_table_head, words[1]);
                    if (new_symbol == NULL)
                    {
                        fprintf(stdout, "Error! in line %d, Entry declaration but no symbol was found.\n", line_number);
                        *passed_second = 0;
                    }
                    else
                    {
                        new_variable = createVariable(new_symbol->name, new_symbol->address, "ent");
                        addVariable(variable_table_head_copy, new_variable);
                    }
                }

                if (strcmp(words[0], ".extern") != 0 && strcmp(words[0], ".entry") != 0)
                {
                    memory_count = promoteMemoryDirective(memory_count, line_copy, num_words, words[0]);
                    memory_count += 1;
                }
            }
            else if (isInstructionName(words[0]))
            {
                *ic += 1;
                calculateInstructionBinary(words, num_words, binary_code_table_head_copy, symbol_table_head_copy, variable_table_head_copy,
                                           extern_table_head_copy, &line_number, passed_second, &memory_count, ic, is_ext);
                memory_count += 1;
            }
        }

        /* Reset words array */
        memset(words, '\0', sizeof(words));
        num_words = 0;

        line_number += 1;
    }

    memory_count -= 1; /* Fix last line */
}