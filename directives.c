#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "directives.h"

/* Create the array of directives */
const struct Directive directiveArray[DIRECTIVES_H] = {
    /* Name, number of operands, decimal code */
    {".data"},
    {".string"},
    {".entry"},
    {".extern"}
};


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



