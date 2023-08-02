#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include <stdbool.h>
#include "globals.h"

struct Directive
{
    const char *name;
};

/* Declare an array of instuctions */
extern const struct Directive directiveArray[NUM_OF_DIRECTIVES];

/* Function to check if a given name is a macro name */
bool isDirectiveName(char *received_name);

#endif /* DIRECTIVES_H */
