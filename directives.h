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

char *getName(char *full_directive_name);

bool dataCommaProblem(char *line, int line_number, int num_words);

/* Validate directive syntax */
bool validDirective(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number);

#endif /* DIRECTIVES_H */
