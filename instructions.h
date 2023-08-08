#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdbool.h>
#include "globals.h"

struct Instruction
{
    const char *name;
    const int operands_num;
    int decimal_code;
    int source_addressing_method[3];
    int destination_addressing_method[3];
};

/* Declare an array of instuctions */
extern const struct Instruction instructionsArray[NUM_OF_INSTRUCTIONS];

/* Function to check if a given name is a macro name */
bool isInstructionName(char *received_name);

bool instructionCommaProblem(char *line, int line_number, int num_words, char *instruction_full_name, int index);

bool instructionOperandsProblem(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number, char *instruction_full_name, int index);

/* Validate instruction syntax */
bool validInstruction(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number);

#endif /* INSTRUCTIONS_H */
