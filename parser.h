#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "macro.h"
#include "symbol.h"
#include "globals.h"
#include "extern.h"
#include "binary.h"
#include "variables.h"

/*
Remove leading spaces from a given string.

@param input The input string to be cleaned.
*/
void cleanLeadingSpaces(char *input);

/*
Remove trailing spaces from a given string.

@param input The input string to be cleaned.
*/
void removeTrailingSpaces(char *input);

/*
Remove all spaces from a given string.

@param param The input string to be cleaned.
*/
void cleanAllSpaces(char *param);

/*
Check if a comma is present in a given string.
 
@param input The input string to be checked.
@return true if a comma is found, otherwise false.
*/
bool commaInLine(char *input);

/*
Check if a comma is present at the first or last character of a string.

@param input The input string to be checked.
@return true if a comma is at the first or last character, otherwise false.
*/
bool commaAtFirstOrLast(char *input);

/*
Check if a string has double quotes at both the start and end.

@param input The input string to be checked.
@return true if the string has double quotes at both ends, otherwise false.
*/
bool quoteAtFirstAndLast(char *input);

/*
Check if a comma appears after the first word in a string.

@param line The input string to be checked.
@param first_word The first word to check after.
@return true if a comma is found after the first word, otherwise false.
*/
bool commaAfterFirstWord(char *line, char *first_word);

/*
Check if there are consecutive commas in a string.

@param line The input string to be checked.
@param line_number The line number in the file.
@return true if consecutive commas are found, otherwise false.
*/
bool doubleComma(char *line, int line_number);

/*
Count the number of commas in a string.

@param line The input string to be checked.
@return The count of commas in the string.
*/
int countCommas(char *line);

/*
Count the number of quotes in a string.

@param line The input string to be checked.
@return The count of quotes in the string.
*/
int countQuotes(char *line);

/*
Remove a newline character from the end of a string.

@param line The input string to be modified.
*/
void removeNewLineFromEnd(char *line);

/*
Check if a line is empty or a comment line.

@param line The input string to be checked.
@return true if the line is empty or a comment, otherwise false.
*/
bool isEmptyOrCommentLines(char *line);

/*
Tokenize a line into individual words.

@param line The input string to be tokenized.
@param words The array to store the tokenized words.
@param numWords The maximum number of words to tokenize.
@return The actual number of words tokenized.
*/
int storeWords(char *line, char words[][MAX_LINE_LENGTH], int numWords);

/*
Tokenize a line into individual words, including handling special cases.

@param line The input string to be tokenized.
@param words The array to store the tokenized words.
@param num_words The maximum number of words to tokenize.
@return The actual number of words tokenized.
*/
int tokenStrings(char *line, char words[][MAX_LINE_LENGTH], int num_words);

/*
Reset the copy of a line back to its original state.

@param line The original line.
@param copy_line The copy of the line to be reset.
*/
void resetLineCopy(char *line, char *copy_line);

/*
Check if a string can be converted to an integer within a specified range.

@param param The string to be checked.
@param min_range The minimum value of the range.
@param max_range The maximum value of the range.
@return true if the string can be converted to an integer within the range, otherwise false.
*/
bool isIntegerInRange(char *param, int min_range, int max_range);

/*
Check if a string contains only ASCII characters.

@param line The string to be checked.
@param line_number The line number in the file.
@return true if the string contains only ASCII characters, otherwise false.
*/
bool isAscii(char *line, int line_number);

/*
Remove a sub-string from a given string.

@param line The input string.
@param sub_string The sub-string to be removed.
*/
void removeSubString(char *line, char *sub_string);

/*
Parse the assembly file, handling macros.
Will find macros and create a macro table, while parsing if finding a known macro,
Write its data to the .am file,
and write all the other lines to the .am file.
Check errors only for macro declaration
Ignore comment lines and empty lines.

@param assembly_file The input assembly file.
@param am_file The temporary assembly file after macro expansion.
@param am_file_name The name of the temporary assembly file.
@param macro_table_head The head of the macro table.
@return true if parsing and macro expansion is successful, otherwise false.
*/
bool parseFileHandleMacros(FILE *assembly_file, FILE *am_file, char *am_file_name, struct Macro *macro_table_head);

/*
Perform the first pass of the assembly process.
Count memory + ic + dc
Create extern table
Create symbol table
Check syntax for all lines
Flag if first pass is ok, and if there are .entry lines

@param am_file The temporary assembly file.
@param symbol_table_head The head of the symbol table.
@param extern_table_head The head of the extern table.
@param passed_first A flag indicating if the first pass succeeded.
@param is_ent A flag indicating if an entry directive is present.
*/
void parseFirstPass(FILE *am_file, struct Symbol *symbol_table_head, struct Extern *extern_table_head, int *passed_first, int *is_ent);

/*
Perform the second pass of the assembly process.
All syntax is correct, need to check for unknown symbols (in extern table)
Re-count memory + ic + dc
Create the binary table with binary code for each line
Create Variables table to later output to .ent .ext files
Flag if second pass is ok, and if there are extern symbols in use.

@param am_file The temporary assembly file.
@param symbol_table_head The head of the symbol table.
@param extern_table_head The head of the extern table.
@param binary_code_head The head of the binary code table.
@param variable_table_head The head of the variable table.
@param passed_second A flag indicating if the second pass succeeded.
@param ic The instruction counter.
@param dc The data counter.
@param is_ext A flag indicating if an extern directive is present.
*/
void parseSecondPass(FILE *am_file, struct Symbol *symbol_table_head, struct Extern *extern_table_head, struct Binary *binary_code_head, struct Variable *variable_table_head, int *passed_second, int *ic, int *dc, int *is_ext);

#endif /* PARSER_H */
