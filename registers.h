#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdbool.h>
#include "globals.h"

struct Register
{
    const char *name;
    int reg_num;
};

/* Declare an array of instuctions */
extern const struct Register registersArray[NUM_OF_REGISTERS];

/* Function to check if a given name is a macro name */
bool isRegisterName(char *received_name);

#endif /* REGISTERS_H */
