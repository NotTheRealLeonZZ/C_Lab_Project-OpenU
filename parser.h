
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "macro.h"

#define MAX_LINE_LENGTH 81 /*  Maximum line length, including '\n' and '\0' */

/* Function to parse the assembly file */
void parseFileGetMacro(FILE *assembly_file, struct Macro *macro_table);

#endif /* PARSER_H */
