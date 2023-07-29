#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "macro.h"

#define MAX_LINE_LENGTH 81 /*  Maximum line length, including '\n' and '\0' */
#define MAX_INSTRUCTION_LENGTH 4 /* Maximum instruction string length */

void cleanLeadingSpaces(char *input);

void cleanAllSpaces(char *param);

bool commaProblem(char *input);

void removeNewLineFromEnd (char *line);

bool isEmptyOrCommentLines(char *line);

int storeWords(char *line, char words[][MAX_LINE_LENGTH], int numWords);



void deleteCurrentLine(FILE *file);

/* Function to parse the assembly file */
void parseFileHandleMacros(FILE *assembly_file, FILE *am_file, char *am_file_name, struct Macro *macro_table_head);

void parseFileSetMacro(FILE *assembly_file, struct Macro *macro_table_head);

#endif /* PARSER_H */
