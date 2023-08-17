#ifndef REGISTERS_H
#define REGISTERS_H

#include "globals.h"

/* Register struct will hold an array of all known register names and their numbers.*/
struct Register
{
    const char *name; /* Register name */
    int reg_num;      /* Register number */
};

/* Declare an array of registers */
extern const struct Register registersArray[NUM_OF_REGISTERS];

/*
Check if a given string is a valid register name.

@param received_name The string to be checked.
@return true if the string is a valid register name, otherwise false.
*/
bool isRegisterName(char *received_name);

/*
Get the register number corresponding to a register name.

@param register_name The register name.
@return The register number, or 0 if the name is not a valid register name.
*/
int getRegisterNum(char register_name[]);

#endif /* REGISTERS_H */
