#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdbool.h>
#include "globals.h"

struct Instruction
{
    const char *name;
    const int operands_num;
    int decimal_code;
};

/* Declare an array of instuctions */
extern const struct Instruction instructionsArray[NUM_OF_INSTRUCTIONS];

/* Function to check if a given name is a macro name */
bool isInstructionName(char *received_name);

#endif /* INSTRUCTIONS_H */
