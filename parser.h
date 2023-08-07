#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "macro.h"
#include "symbol.h"
#include "globals.h"

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

bool isInteger(char *param, int line_number);

bool isAscii(char *line, int line_number);

void removeSubString(char *line, char *sub_string);

/* Function to parse the assembly file */
bool parseFileHandleMacros(FILE *assembly_file, FILE *am_file, char *am_file_name, struct Macro *macro_table_head);

void parseFileHandleSymbols(FILE *assembly_file, struct Symbol *symbol_table_head);

#endif /* PARSER_H */
