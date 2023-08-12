#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include <stdbool.h>
#include "globals.h"
#include "symbol.h"
#include "binary.h"
#include "variables.h"

struct Directive
{
    const char *name;
};

/* Declare an array of instuctions */
extern const struct Directive directiveArray[NUM_OF_DIRECTIVES];

/* Function to check if a given name is a macro name */
bool isDirectiveName(char *received_name);

char *getName(char *full_directive_name);

bool dataCommaProblem(char *line, int line_number, int num_words, char *directive_full_name);

bool stringCommaProblem(char *line, int line_number, char *directive_full_name);

bool stringQuoteasProblem(char *line, int line_number, char *directive_full_name);

/* Validate directive syntax */
bool validDirective(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number);

void calculateDirectiveBinary(char words[][MAX_LINE_LENGTH], int num_words, struct Binary *binary_code_table_head, struct Symbol *symbol_table_head, struct Variable *variable_table_head);

void calculateDataBinary(char words[][MAX_LINE_LENGTH], int num_words, struct Binary *binary_code_table_head);

void calculateStringBinary(char words[][MAX_LINE_LENGTH], struct Binary *binary_code_table_head);

#endif /* DIRECTIVES_H */
