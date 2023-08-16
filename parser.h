#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "macro.h"
#include "symbol.h"
#include "globals.h"
#include "extern.h"
#include "binary.h"
#include "variables.h"

void cleanLeadingSpaces(char *input);

void removeTrailingSpaces(char *input);

void cleanAllSpaces(char *param);

bool commaInLine(char *input);

bool commaAtFirstOrLast(char *input);

bool quoteAtFirstAndLast(char *input);

bool commaAfterFirstWord(char *line, char *first_word);

bool doubleComma(char *line, int line_number);

int countCommas(char *line);

int countQuotes(char *line);

void removeNewLineFromEnd(char *line);

bool isEmptyOrCommentLines(char *line);

int storeWords(char *line, char words[][MAX_LINE_LENGTH], int numWords);

int tokenStrings(char *line, char words[][MAX_LINE_LENGTH], int num_words);

void deleteCurrentLine(FILE *file);

void resetLineCopy(char *line, char *copy_line);

bool isIntegerInRange(char *param, int min_range, int max_range);

bool isAscii(char *line, int line_number);

void removeSubString(char *line, char *sub_string);

/* Function to parse the assembly file */
bool parseFileHandleMacros(FILE *assembly_file, FILE *am_file, char *am_file_name, struct Macro *macro_table_head);

void parseFileHandleSymbols(FILE *am_file, struct Symbol *symbol_table_head, struct Extern *extern_table_head, int *passed_first);

void parseSecondPass(FILE *am_file, struct Symbol *symbol_table_head, struct Extern *extern_table_head, struct Binary *binary_code_head, struct Variable *variable_table_head, int *passed_second, int *ic, int *dc);

#endif /* PARSER_H */
