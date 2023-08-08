#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "instructions.h"
#include "globals.h"
#include "parser.h"

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
        printf("No commas after instruction!\n");
        if (!doubleComma(line_copy, line_number))
        {
            /* Count commas in line */
            comma_count = countCommas(line_copy);
            if (instructionsArray[index].operands_num == 2)
                instruction_correct_comma_count = 1;

            printf("current line has %d commas and should have %d\n", comma_count, instruction_correct_comma_count);

            if (instruction_correct_comma_count == comma_count)
            {
                printf("CORRECT number of commas for this line.\n");
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
    printf("instruction_correct_operands_count: %d\n", instruction_correct_operands_count);
    if (instructionsArray[index].operands_num == instruction_correct_operands_count)
    {
        printf("correct number of operands!\n");
        return false;
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
                printf("number of operands for this instruction are %d\n", instructionsArray[i].operands_num);
                if (!instructionOperandsProblem(words, num_words, line, line_number, instruction_full_name, i))
                {
                    printf("operands are correct!\n");
                    return true;
                }
            }
        }
    }

    return false;
}
