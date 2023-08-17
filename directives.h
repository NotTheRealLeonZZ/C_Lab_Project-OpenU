#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include "globals.h"
#include "symbol.h"
#include "binary.h"
#include "variables.h"

/*
Directive struct will hold all directive names. (.data, .string, .entry, .extern)
Will use this to create the nodes and make all checks needed that is directive related
.data - only integers that can be encoded to binary 12 bit. num of operands until max of line.
.string - single string between 2 quotes, Cannot have quotes inside the string. (accourding to Forum)
.extern - single operand which is a symbol from different file, make sure its valid symbol name.
.entry - single operand which is a symbol from current file that can be used outside of this file.
*/

struct Directive
{
    const char *name;
};

/* Declare an array of instuctions */
extern const struct Directive directiveArray[NUM_OF_DIRECTIVES];

/*
Function to check if a given name is a macro name

@param received_name The name to be checked.
@return True if it is a directive name, otherwise false.
*/
bool isDirectiveName(char *received_name);

/*
Check for the presence of a comma problem in a data directive.

@param line The line containing the directive.
@param line_number The line number.
@param num_words The number of words in the line.
@param directive_full_name The full name of the directive.
@return True if a problem exists, otherwise false.
*/
bool dataCommaProblem(char *line, int line_number, int num_words, char *directive_full_name);

/*
Check for the presence of a comma problem in a string directive.

@param line The line containing the directive.
@param line_number The line number.
@param directive_full_name The full name of the directive.
@return True if a problem exists, otherwise false.
*/

bool stringCommaProblem(char *line, int line_number, char *directive_full_name);

/*
Check for the presence of a quote problem in a string directive.
There should be only 2 quotes, at the start and at the end of the string.

@param line The line containing the directive.
@param line_number The line number.
@param directive_full_name The full name of the directive.
@return True if a problem exists, otherwise false.
*/
bool stringQuoteasProblem(char *line, int line_number, char *directive_full_name);

/*
Validate the syntax of a directive.

@param words The array of words in the line.
@param num_words The number of words in the line.
@param line The line containing the directive.
@param line_number The line number.
@return True if the syntax is valid, otherwise false.
*/
bool validDirective(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number);

/*
Calculate the binary representation of a directive.

@param words The array of words in the line.
@param num_words The number of words in the line.
@param binary_code_table_head The head of the binary code table.
*/
void calculateDirectiveBinary(char words[][MAX_LINE_LENGTH], int num_words, struct Binary *binary_code_table_head);

/*
Calculate the binary representation of a data directive.
Adds a node at the end of the binary linked list.

@param words The array of words in the line.
@param num_words The number of words in the line.
@param binary_code_table_head The head of the binary code table.
*/
void calculateDataBinary(char words[][MAX_LINE_LENGTH], int num_words, struct Binary *binary_code_table_head);

/*
Calculate the binary representation of a string directive.
Adds a node at the end of the binary linked list.

@param words The array of words in the line.
@param binary_code_table_head The head of the binary code table.
*/
void calculateStringBinary(char words[][MAX_LINE_LENGTH], struct Binary *binary_code_table_head);

#endif /* DIRECTIVES_H */
