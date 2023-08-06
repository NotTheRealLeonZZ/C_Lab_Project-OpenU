#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "directives.h"
#include "globals.h"
#include "parser.h"

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
        printf("got directive name: %s\n", full_directive_name + 1);
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
        if (count_quotes != 2)
        {
            fprintf(stdout, "Error! in line %d, Incorrect number of quotes.\n", line_number);
            return true;
        }
    }

    return false;
}

/* Validate directive syntax */
bool validDirective(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number)
{
    char *directive_full_name = words[0];
    int i;
    int num_count = 0;

    if (strcmp(directive_full_name, ".data") == 0)
    {
        printf("This is a data directive!\n");

        /* Check syntax for data */
        if (!dataCommaProblem(line, line_number, num_words, directive_full_name))
        {
            for (i = 1; i < num_words; i++)
            {
                if (isInteger(words[i], line_number))
                    num_count += 1;
            }

            if (num_count == num_words - 1)
                return true;
        }
    }

    else if (strcmp(directive_full_name, ".string") == 0)
    {
        printf("This is a string directive!\n");

        /* Commas are allowed, only between "" */
        if (!stringCommaProblem(line, line_number, directive_full_name))
        {
            if (!stringQuoteasProblem(line, line_number, directive_full_name))
            {
                /* No problem with quotes */
                printf("Line to check quotes: %s\n", line);
                if (isAscii(line, line_number))
                    return true;
            }
        }
    }

    else if (strcmp(directive_full_name, ".entry") == 0)
    {
        printf("This is an entry directive!\n");
        return false; /*change to true */
    }

    else if (strcmp(directive_full_name, ".extern") == 0)
    {
        printf("This is an extern directive!\n");
        return false; /*change to true */
    }

    return false;
}