#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "directives.h"
#include "globals.h"
#include "parser.h"
#include "instructions.h"
#include "registers.h"
#include "encoding.h"
#include "symbol.h"

/* Create the array of directives */
const struct Directive directiveArray[NUM_OF_DIRECTIVES] = {
    /* Name, number of operands, decimal code */
    {".data"},
    {".string"},
    {".entry"},
    {".extern"}};

/* Function to check if a given name is a macro name */
bool isDirectiveName(char *received_name)
{
    int i;
    for (i = 0; i < NUM_OF_DIRECTIVES; i++)
    {
        if (strcmp(received_name, directiveArray[i].name) == 0)
        {
            return true;
        }
    }
    return false;
}

/* Get hte name of directive without the dot
maybe delete if no need */
char *getName(char *full_directive_name)
{
    if (full_directive_name == NULL || *full_directive_name == '\0')
        return NULL;

    if (isDirectiveName(full_directive_name))
    {

        return full_directive_name + 1;
    }

    return NULL;
}

bool dataCommaProblem(char *line, int line_number, int num_words, char *directive_full_name)
{
    char line_copy[MAX_LINE_LENGTH];
    int comma_count;
    int data_correct_comma_count = num_words - 2; /* Sum words minus 1 for the .data word, and should be 1 less than parameters */
    char directive_name_copy[NUM_OF_CHARACTERS_FOR_DIRECTIVE];

    strcpy(directive_name_copy, directive_full_name);
    strcpy(line_copy, line);

    /* Check there is no comma after .data */
    if (!commaAfterFirstWord(line_copy, directive_name_copy))
    {

        /* Check for double commas in line */
        if (!doubleComma(line_copy, line_number))
        {
            /* Count commas in line */
            comma_count = countCommas(line_copy);

            if (comma_count == data_correct_comma_count)
            {
                /* Corrent number of commas. */
                return false;
            }
            else
            {
                fprintf(stdout, "Error! in line %d: Incorrect number of commas.\n", line_number);
            }
        }
    }
    else
    {
        fprintf(stdout, "Error! in line %d: Invalid comma in line.\n", line_number);
    }

    return true;
}

bool stringCommaProblem(char *line, int line_number, char *directive_full_name)
{
    char line_copy[MAX_LINE_LENGTH];
    char directive_name_copy[NUM_OF_CHARACTERS_FOR_DIRECTIVE];

    strcpy(directive_name_copy, directive_full_name);
    strcpy(line_copy, line);

    if (!commaAfterFirstWord(line_copy, directive_name_copy)) /* Modify the real line */
    {
        return false;
    }
    else
    {
        fprintf(stdout, "Error! in line %d: Invalid comma in line.\n", line_number);
    }
    return true;
}

bool stringQuoteasProblem(char *line, int line_number, char *directive_full_name)
{
    int count_quotes = 0;
    removeSubString(line, directive_full_name);
    cleanLeadingSpaces(line);

    /* Check if quote is 1st and last char
    Check if there are exactly 2 Apostro 
    Check all characters are ascii*/

    if (quoteAtFirstAndLast(line))
    {
        count_quotes = countQuotes(line);

        if (count_quotes == 2)
        {

            return false;
        }
    }
    /* fprintf(stdout, "Error! in line %d, Incorrect number of quotes.\n", line_number); */

    return true;
}

/* Validate directive syntax */
bool validDirective(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number)
{
    char *directive_full_name = words[0];
    int i;
    int num_count = 0;

    if (strcmp(directive_full_name, ".data") == 0)
    {

        /* Check syntax for data */
        if (!dataCommaProblem(line, line_number, num_words, directive_full_name))
        {
            for (i = 1; i < num_words; i++)
            {
                if (isIntegerInRange(words[i], INT_DIRECTIVE_MIN_RANGE, INT_DIRECTIVE_MAX_RANGE))
                    num_count += 1;

                else
                {
                    fprintf(stdout, "Error! in line %d: Parameter %s should have been an integer in range and it's not. (-2048 to 2047)\n", line_number, words[i]);
                    return false;
                }
            }

            if (num_count == num_words - 1)
                return true;
        }
    }

    else if (strcmp(directive_full_name, ".string") == 0)
    {

        /* Commas are allowed, only between "" */
        if (!stringCommaProblem(line, line_number, directive_full_name))
        {
            if (!stringQuoteasProblem(line, line_number, directive_full_name))
            {
                /* No problem with quotes */

                if (isAscii(line, line_number))
                    return true;
            }
            else
            {
                fprintf(stdout, "Error! in line: %d, Number of quotes is illegal.\n", line_number);
            }
        }
    }

    /* Entry and Extern have only 1 parameter */
    else if (strcmp(directive_full_name, ".entry") == 0 || strcmp(directive_full_name, ".extern") == 0)
    {
        if (!commaInLine(line))
        {

            if (num_words == 2)
            {

                if (!isDirectiveName(words[1]) && !isInstructionName(words[1]) && !isRegisterName(words[1]))
                {

                    return true;
                }
                else
                {
                    fprintf(stdout, "Error! in line %d, Invalid parameter.\n", line_number);
                }
            }
            else
            {
                fprintf(stdout, "Error! in line: %d, Too many arguments.\n", line_number);
            }
        }
        else
        {
            fprintf(stdout, "Error! in line %d, Invalid comma in line.\n", line_number);
        }
    }

    return false;
}

void calculateDirectiveBinary(char words[][MAX_LINE_LENGTH], int num_words, struct Binary *binary_code_table_head, struct Symbol *symbol_table_head, struct Variable *variable_table_head)
{
    if (strcmp(words[0], ".data") == 0)
    {
        calculateDataBinary(words, num_words, binary_code_table_head);
    }
    else if (strcmp(words[0], ".string") == 0)
    {
        calculateStringBinary(words, binary_code_table_head);
    }

    /* If its .entry, no need to calculate binary. */
    /* If its .extern I already handled it. */
}

void calculateDataBinary(char words[][MAX_LINE_LENGTH], int num_words, struct Binary *binary_code_table_head)
{
    char *binary_encode;
    int i;
    struct Binary *binary_code_table_head_copy; /* A copy of the binary_code table head node, to manipulate without losing the original pointer */
    struct Binary *new_binary_code;             /* New binary_code to add to the binary_code table */

    binary_code_table_head_copy = binary_code_table_head;

    for (i = 1; i < num_words; i++)
    {
        binary_encode = encodeStrIntToBinary(words[i], BINARY_CODE_LENGTH);
        new_binary_code = createBinary(binary_encode, "dir");
        addBinary(binary_code_table_head_copy, new_binary_code);
    }

    free(binary_encode);
}

void calculateStringBinary(char words[][MAX_LINE_LENGTH], struct Binary *binary_code_table_head)
{
    const char *string = words[1];
    size_t length = strlen(string);
    int i;
    char *binary_char;
    struct Binary *binary_code_table_head_copy; /* A copy of the binary_code table head node, to manipulate without losing the original pointer */
    struct Binary *new_binary_code;             /* New binary_code to add to the binary_code table */

    binary_code_table_head_copy = binary_code_table_head;

    /* Parse the string wihtout the quotes */
    for (i = 0; i < length; i++)
    {
        binary_char = encodeCharToBinary(string[i]);
        new_binary_code = createBinary(binary_char, "dir");
        addBinary(binary_code_table_head_copy, new_binary_code);
    }

    binary_char = encodeIntToBinary(0, BINARY_CODE_LENGTH);
    new_binary_code = createBinary(binary_char, "dir");
    addBinary(binary_code_table_head_copy, new_binary_code);

    free(binary_char);
}