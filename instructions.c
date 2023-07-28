#include <stdbool.h>
#include <string.h>
#include "instructions.h"

int countWords(char *line)
{
    return -1;
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
