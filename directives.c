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

bool dataCommaProblem(char *line, int line_number, int num_words)
{
    char line_copy[MAX_LINE_LENGTH];
    int comma_count;
    int data_correct_comma_count = num_words - 2; /* Sum words minus 1 for the .data word, and should be 1 less than parameters */

    strcpy(line_copy, line);
    printf("line in dataCommaProblem; %s\n", line);

    /* Check for double commas in line */
    if (!doubleComma(line_copy, line_number))
    {
        /* Count commas in line */
        comma_count = countCommas(line_copy);
        printf("number of commas in line: %d while it should be: %d\n", comma_count, data_correct_comma_count);
        if (comma_count == data_correct_comma_count)
        {
            /* Corrent number of commas.
            Validate parameters to integers only! */
            printf("Corrent number of commas for line: %s\n", line);
        }
    }
    return true;
}

/* Validate directive syntax */
bool validDirective(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number)
{
    char *directive_full_name = words[0];

    printf("directive_full_name in validDirective: %s\n", directive_full_name);
    printf("line in validDirective: %s\n", line);

    if (strcmp(directive_full_name, ".data") == 0)
    {
        printf("This is a data directive!\n");

        /* Check syntax for data */
        if (!dataCommaProblem(line, line_number, num_words))
            return true;
    }
    else if (strcmp(directive_full_name, ".string") == 0)
    {
        printf("This is a string directive!\n");
        return true;
    }
    else if (strcmp(directive_full_name, ".entry") == 0)
    {
        printf("This is an entry directive!\n");
        return true;
    }
    else if (strcmp(directive_full_name, ".extern") == 0)
    {
        printf("This is an extern directive!\n");
        return true;
    }
    return false;
}