#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "registers.h"
#include "globals.h"

/* Create the array of instructions */
const struct Register registersArray[NUM_OF_REGISTERS] = {
    /* Name, Register number (For later encoding) */
    {"@r0", 0},
    {"@r1", 1},
    {"@r2", 2},
    {"@r3", 3},
    {"@r4", 4},
    {"@r5", 5},
    {"@r6", 6},
    {"@r7", 7},

};

/* Function to check if a given name is a macro name */
bool isRegisterName(char *received_name)
{
    int i;
    for (i = 0; i < NUM_OF_REGISTERS; i++)
    {
        if (strcmp(received_name, registersArray[i].name) == 0)
        {
            return true;
        }
    }
    return false;
}
