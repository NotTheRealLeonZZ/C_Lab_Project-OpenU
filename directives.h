#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include <stdbool.h>

/* Define number of instructions */
#define NUM_OF_DIRECTIVES 4

struct Directive
{
    const char *name;

};

/* Declare an array of instuctions */
extern const struct Directive directiveArray[DIRECTIVES_H];

/* Function to check if a given name is a macro name */
bool isDirectiveName(char *received_name);



#endif /* DIRECTIVES_H */
