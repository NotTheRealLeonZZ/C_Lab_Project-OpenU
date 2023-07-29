#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "instructions.h"

/* Create the array of instructions */
const struct Instruction instructionsArray[NUM_OF_INSTRUCTIONS] = {

    {"mov", 2, 0},
    {"cmp", 2, 1},
    {"add", 2, 2},
    {"sub", 2, 3},
    {"lea", 2, 6},
    {"not", 1, 4},
    {"clr", 1, 5},
    {"inc", 1, 7},
    {"dec", 1, 8},
    {"jmp", 1, 9},
    {"bne", 1, 10},
    {"red", 1, 11},
    {"prn", 1, 12},
    {"jsr", 1, 13},
    {"rts", 0, 14},
    {"stop", 0, 15}

};


/* Function to check if a given name is a macro name */
bool isInstructionName(char *received_name)
{
    int i;
    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {
        if (strcmp(received_name, instructionsArray[i].name) == 0)
        {
            return true;
        }
    }
    return false;
}



bool validateInstruction(char *instruction)
{
    /* Parse the instruction and its arguments from the input string 
    /* For example, you can use strtok to tokenize the input string
    /* Check for instructions with 1 param, 2 param and 0 param */
    char *instrName = strtok(instruction, " ,");
    char *arg1 = strtok(NULL, " ,");
    char *arg2 = strtok(NULL, " ,");

    /* Validate the instruction name */
    if (strcmp(instrName, "mov") == 0)
    {
        /* Validate arguments for the MOV instruction
        /* Return true if the arguments are valid for this instruction, otherwise false
        /* Example validation logic: */
        if (arg1 != NULL && arg2 != NULL && arg1[0] == '@' && arg2[0] == '@')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (strcmp(instrName, "add") == 0)
    {
        /* Validate arguments for the ADD instruction
        /* Return true if the arguments are valid for this instruction, otherwise false
        /* Example validation logic: */
        if (arg1 != NULL && arg2 != NULL && arg1[0] == '@' && arg2[0] == '@')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (strcmp(instrName, "sub") == 0)
    {
        /* Validate arguments for the SUB instruction
        /* Return true if the arguments are valid for this instruction, otherwise false
        /* Example validation logic: */
        if (arg1 != NULL && arg2 != NULL && arg1[0] == '@' && arg2[0] == '@')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    /* Add more cases for other instructions
    /* ...

    /* If the instruction name is not recognized, return false */
    return false;
}
