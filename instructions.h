#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdbool.h>
#include "globals.h"
#include "symbol.h"
#include "variables.h"
#include "extern.h"
#include "binary.h"

struct Instruction
{
    const char *name;
    const int operands_num;
    int decimal_code;
    int source_addressing_method[NUM_OF_ADDRESSING_METHODS];
    int destination_addressing_method[NUM_OF_ADDRESSING_METHODS];
};

/* Declare an array of instuctions */
extern const struct Instruction instructionsArray[NUM_OF_INSTRUCTIONS];

int get_decimal_code(char *received_name);

/* Function to check if a given name is a macro name */
bool isInstructionName(char *received_name);

bool validateOneOperandDest(char *operand, int index, int line_number);

bool validateOneOperandSource(char *operand, int index, int line_number);

bool instructionCommaProblem(char *line, int line_number, int num_words, char *instruction_full_name, int index);

bool instructionOperandsProblem(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number,
                                char *instruction_full_name, int index);

/* Validate instruction syntax */
bool validInstruction(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number);

void calculateInstructionBinary(char words[][MAX_LINE_LENGTH], int num_words, struct Binary *binary_code_table_head,
                                struct Symbol *symbol_table_head, struct Variable *variable_table_head, struct Extern *extern_table_head,
                                int *line_number, int *passed_second, int *current_memory, int *ic);

/* add all strings to result to complete a binary word */
void combine4Strings(char *result, const char *are, const char *dest_addressing, const char *instruction_code, const char *source_addressing);

void combine3Strings(char *result, const char *are, const char *operand1, const char *operand2);

void combine2Strings(char *result, const char *are, const char *operand);

#endif /* INSTRUCTIONS_H */
