#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "instructions.h"
#include "globals.h"
#include "parser.h"
#include "registers.h"

/* Create the array of instructions */
const struct Instruction instructionsArray[NUM_OF_INSTRUCTIONS] = {
    /* Name, number of operands, decimal code, source_addressing_method, destination_addressing_method */
    {"mov", 2, 0, {1, 3, 5}, {0, 3, 5}},
    {"cmp", 2, 1, {1, 3, 5}, {1, 3, 5}},
    {"add", 2, 2, {1, 3, 5}, {0, 3, 5}},
    {"sub", 2, 3, {1, 3, 5}, {0, 3, 5}},
    {"lea", 2, 6, {0, 3, 0}, {0, 3, 5}},
    {"not", 1, 4, {0, 0, 0}, {0, 3, 5}},
    {"clr", 1, 5, {0, 0, 0}, {0, 3, 5}},
    {"inc", 1, 7, {0, 0, 0}, {0, 3, 5}},
    {"dec", 1, 8, {0, 0, 0}, {0, 3, 5}},
    {"jmp", 1, 9, {0, 0, 0}, {0, 3, 5}},
    {"bne", 1, 10, {0, 0, 0}, {0, 3, 5}},
    {"red", 1, 11, {0, 0, 0}, {0, 3, 5}},
    {"prn", 1, 12, {0, 0, 0}, {1, 3, 5}},
    {"jsr", 1, 13, {0, 0, 0}, {0, 3, 5}},
    {"rts", 0, 14, {0, 0, 0}, {0, 0, 0}},
    {"stop", 0, 15, {0, 0, 0}, {0, 0, 0}}

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

/* This method is for first pass, where my symbol table is not yet full */
bool validateOneOperandDest(char *operand, int index, int line_number)
{
    bool isInt = isInteger(operand);

    size_t operand_length = strlen(operand);
    operand[operand_length] = ':';
    bool isSymbol = wordIsSymbol(operand);
    operand[operand_length] = '\0';

    bool isRegister = isRegisterName(operand);

    if (isInt && instructionsArray[index].destination_addressing_method[0] != 0)
        return true;
    if (isSymbol && instructionsArray[index].destination_addressing_method[1] != 0)
        return true;
    if (isRegister && instructionsArray[index].destination_addressing_method[2] != 0)
        return true;
    fprintf(stdout, "Error! in line: %d, Incorrect operands for given instruction.\n", line_number);
    return false;
}

/* This method is for first pass, where my symbol table is not yet full */
bool validateOneOperandSource(char *operand, int index, int line_number)
{
    bool isInt = isInteger(operand);

    size_t operand_length = strlen(operand);
    operand[operand_length] = ':';
    bool isSymbol = wordIsSymbol(operand);
    operand[operand_length] = '\0';

    bool isRegister = isRegisterName(operand);

    if (isInt && instructionsArray[index].source_addressing_method[0] != 0)
        return true;
    if (isSymbol && instructionsArray[index].source_addressing_method[1] != 0)
        return true;
    if (isRegister && instructionsArray[index].source_addressing_method[2] != 0)
        return true;
    return false;
}

bool instructionCommaProblem(char *line, int line_number, int num_words, char *instruction_full_name, int index)
{
    char line_copy[MAX_LINE_LENGTH];
    int comma_count;
    int instruction_correct_comma_count = 0;
    char instruction_name_copy[NUM_OF_CHARACTERS_FOR_DIRECTIVE];

    strcpy(instruction_name_copy, instruction_full_name);
    strcpy(line_copy, line);

    /* Check there is no comma after instruction name */
    if (!commaAfterFirstWord(line_copy, instruction_name_copy))
    {
        if (!doubleComma(line_copy, line_number))
        {
            /* Count commas in line */
            comma_count = countCommas(line_copy);
            if (instructionsArray[index].operands_num == 2)
                instruction_correct_comma_count = 1;

            if (instruction_correct_comma_count == comma_count)
            {
                /* Correct number of commas */
                return false;
            }
            else
            {
                fprintf(stdout, "Error! in line %d, Incorrect number of commas.\n", line_number);
            }
        }
    }
    else
    {
        fprintf(stdout, "Error! in line %d: Invalid comma in line.\n", line_number);
    }
    return true;
}

bool instructionOperandsProblem(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number, char *instruction_full_name, int index)
{

    int instruction_correct_operands_count = num_words - 1;
    if (instructionsArray[index].operands_num == instruction_correct_operands_count)
    {
        /* Correct number of operands */
        if (instruction_correct_operands_count == 2)
        {
            /* That means that words[1] is source operand and words[2] is destination operand */
            if (validateOneOperandSource(words[1], index, line_number) && validateOneOperandDest(words[2], index, line_number))
            {
                return false;
            }
        }
        else if (instruction_correct_operands_count == 1)
        {
            /* This means words[1] is destination operand */
            if (validateOneOperandDest(words[1], index, line_number))
            {
                return false;
            }
        }
        else
        {
            /* This means there are no operands at all */
            return false;
        }
    }
    else
    {
        fprintf(stdout, "Error! in line %d, Incorrect number of operands.\n", line_number);
    }
    return true;
}

bool validInstruction(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number)
{
    char *instruction_full_name = words[0];
    int i;

    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {
        if (strcmp(instruction_full_name, instructionsArray[i].name) == 0)
        {
            if (!instructionCommaProblem(line, line_number, num_words, instruction_full_name, i))
            {
                if (!instructionOperandsProblem(words, num_words, line, line_number, instruction_full_name, i))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }

    return false;
}
