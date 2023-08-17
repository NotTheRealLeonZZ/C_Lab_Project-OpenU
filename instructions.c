
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"
#include "globals.h"
#include "parser.h"
#include "registers.h"
#include "encoding.h"
#include "binary.h"
#include "extern.h"
#include "symbol.h"
#include "variables.h"

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

int get_decimal_code(char *received_name)
{
    int i;
    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    {
        if (strcmp(received_name, instructionsArray[i].name) == 0)
        {
            return instructionsArray[i].decimal_code;
        }
    }
    return -1;
}

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
    bool isSymbol;
    bool isRegister;
    bool isInt = isIntegerInRange(operand, INT_DIRECTIVE_MIN_RANGE, INT_DIRECTIVE_MAX_RANGE);

    size_t operand_length = strlen(operand);
    operand[operand_length] = ':';
    isSymbol = wordIsSymbol(operand);
    operand[operand_length] = '\0';

    isRegister = isRegisterName(operand);

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
    bool isSymbol;
    bool isRegister;
    bool isInt = isIntegerInRange(operand, INT_DIRECTIVE_MIN_RANGE, INT_DIRECTIVE_MAX_RANGE);

    size_t operand_length = strlen(operand);
    operand[operand_length] = ':';
    isSymbol = wordIsSymbol(operand);
    operand[operand_length] = '\0';

    isRegister = isRegisterName(operand);

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
            /* Check if need to add error message */
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

void calculateInstructionBinary(char words[][MAX_LINE_LENGTH], int num_words, struct Binary *binary_code_table_head,
                                struct Symbol *symbol_table_head, struct Variable *variable_table_head, struct Extern *extern_table_head,
                                int *line_number, int *passed_second, int *current_memory, int *ic)
{
    char current_are[SIZE_OF_ARE]; /* A.R.E of current line, will use for binary encode */
    char final_line_encode[BINARY_CODE_LENGTH];
    char *current_instruction_encode = '\0'; /* instruction code of current line, will use for binary encode */
    char *current_are_encode = '\0';
    char *current_source_addressing_encode = '\0';
    char *current_dest_addressing_encode = '\0';
    char *operand_encode = '\0';
    char *operand_encode_2 = '\0';
    int source_register_num = -1;
    int dest_register_num = -1;
    int temp_instruction_code = -1;   /* Temp variable to hold instruction code as decimal */
    int operands_num = num_words - 1; /* Number of operands */
    struct Binary *new_binary_code;
    struct Symbol *new_symbol_dest;   /* New symbol to add to the symbol table */
    struct Symbol *new_symbol_source; /* New symbol to add to the symbol table */
    struct Variable *new_variable;
    char operand_destination[MAX_SYMBOL_NAME_LENGTH];
    char operand_source[MAX_SYMBOL_NAME_LENGTH];
    int source_symbol_address = -1;
    int dest_symbol_address = -1;

    final_line_encode[0] = '\0';
    strcpy(current_are, ABSOLUTE_ARE);

    temp_instruction_code = get_decimal_code(words[0]);

    current_instruction_encode = encodeIntToBinary(temp_instruction_code, SIZE_OF_INSTRUCTION_CODE);

    if (operands_num == 2)
    {
        strcpy(operand_destination, words[2]);
        strcpy(operand_source, words[1]);

        new_symbol_source = findSymbol(symbol_table_head, operand_source);
        new_symbol_dest = findSymbol(symbol_table_head, operand_destination);

        /* Checks source operand type */
        if (isRegisterName(operand_source) && isRegisterName(operand_destination))
        {
            current_are_encode = ABSOLUTE_ARE;
            current_source_addressing_encode = ADDRESSING5;
            current_dest_addressing_encode = ADDRESSING5;
            combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
            new_binary_code = createBinary(final_line_encode, "ins");
            addBinary(binary_code_table_head, new_binary_code);

            /* Calculating 1 extra words because of source and destination addressing both registers */

            strcpy(operand_destination, words[2]);
            source_register_num = getRegisterNum(operand_source);
            encodeRegisterBonusWord(operand_source, operand_destination, final_line_encode, current_are_encode, source_register_num, dest_register_num, operand_encode_2, operand_encode, new_binary_code, binary_code_table_head);

            *ic -= 1;
            *current_memory -= 1;
            *line_number -= 1;
        }
        else if (isIntegerInRange(operand_source, INT_INSTRUCTION_MIN_RANGE, INT_INSTRUCTION_MAX_RANGE))
        {
            /* Source operand is int */

            current_are_encode = ABSOLUTE_ARE;
            current_source_addressing_encode = ADDRESSING1;

            /* Checks destination operand type */
            if (isIntegerInRange(operand_destination, INT_INSTRUCTION_MIN_RANGE, INT_INSTRUCTION_MAX_RANGE))
            {
                /* Both operands are numbers */

                current_dest_addressing_encode = ADDRESSING1;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeIntBonusWord(operand_source, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeIntBonusWord(operand_destination, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
            }
            else if (isRegisterName(operand_destination))
            {
                /* Source is number, destination is register */

                current_dest_addressing_encode = ADDRESSING5;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeIntBonusWord(operand_source, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */

                strcpy(operand_destination, words[2]);
                source_register_num = 0;
                encodeRegisterBonusWord(operand_source, operand_destination, final_line_encode, current_are_encode, source_register_num, dest_register_num, operand_encode_2, operand_encode, new_binary_code, binary_code_table_head);
            }
            else if (new_symbol_dest)
            {
                /* Source is number, destination is known symbol */

                current_dest_addressing_encode = ADDRESSING3;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeIntBonusWord(operand_source, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeSymbolBonusWord(operand_destination, final_line_encode, current_are_encode, dest_symbol_address, new_symbol_dest, operand_encode, new_binary_code, binary_code_table_head);
            }

            else if (findExtern(extern_table_head, operand_destination))
            {
                /* Add to variable table as extern */
                new_variable = createVariable(operand_destination, MEMORY_START + *ic, "ext");
                addVariable(variable_table_head, new_variable);

                current_dest_addressing_encode = ADDRESSING3;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeIntBonusWord(operand_source, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeExternBonusWord(operand_destination, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
            }
            else
            {
                /* Source operand is not int/register/entry symbol or extern symbol. */
                *passed_second = 0;
                fprintf(stdout, "Error! in line: %d, Operand is not known.\n", *line_number);
            }
        }
        else if (isRegisterName(operand_source))
        {
            /* Source operand is register, already checked if dest also register so can assume its not */
            current_are_encode = ABSOLUTE_ARE;
            current_source_addressing_encode = ADDRESSING5;

            /* Check dest operand type */
            if (isIntegerInRange(operand_destination, INT_INSTRUCTION_MIN_RANGE, INT_INSTRUCTION_MAX_RANGE))
            {
                /* Dest operand is int */
                current_dest_addressing_encode = ADDRESSING1;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                source_register_num = getRegisterNum(operand_source);
                encodeRegisterBonusWord(operand_source, operand_destination, final_line_encode, current_are_encode, source_register_num, dest_register_num, operand_encode_2, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_source, words[2]);
                encodeIntBonusWord(operand_source, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
            }
            else if (new_symbol_dest)
            {
                current_dest_addressing_encode = ADDRESSING3;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                source_register_num = getRegisterNum(operand_source);
                encodeRegisterBonusWord(operand_source, operand_destination, final_line_encode, current_are_encode, source_register_num, dest_register_num, operand_encode_2, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeSymbolBonusWord(operand_destination, final_line_encode, current_are_encode, dest_symbol_address, new_symbol_dest, operand_encode, new_binary_code, binary_code_table_head);
            }
            else if (findExtern(extern_table_head, operand_destination))
            {
                /* Dest operand is an extern symbol */

                /* Add to variable table as extern */
                new_variable = createVariable(operand_destination, MEMORY_START + *ic, "ext");
                addVariable(variable_table_head, new_variable);

                current_dest_addressing_encode = ADDRESSING3;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                source_register_num = getRegisterNum(operand_source);
                encodeRegisterBonusWord(operand_source, operand_destination, final_line_encode, current_are_encode, source_register_num, dest_register_num, operand_encode_2, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeExternBonusWord(operand_destination, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
            }
            else
            {
                /* Destination operand is not int/register/entry symbol or extern symbol. */
                *passed_second = 0;
                fprintf(stdout, "Error! in line: %d, Operand is not known.\n", *line_number);
            }
        }
        else if (new_symbol_source)
        {
            /* Source operand is symbol */

            /* Source operand is register, already checked if dest also register so can assume its not */
            current_are_encode = ABSOLUTE_ARE;
            current_source_addressing_encode = ADDRESSING3;

            /* Check dest operand type */
            if (isIntegerInRange(operand_destination, INT_INSTRUCTION_MIN_RANGE, INT_INSTRUCTION_MAX_RANGE))
            {
                /* Dest operand is int */
                current_dest_addressing_encode = ADDRESSING1;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeSymbolBonusWord(operand_source, final_line_encode, current_are_encode, source_symbol_address, new_symbol_source, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeIntBonusWord(operand_destination, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
            }
            else if (isRegisterName(operand_destination))
            {
                /* Dest operand is register */
                current_dest_addressing_encode = ADDRESSING5;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeSymbolBonusWord(operand_source, final_line_encode, current_are_encode, source_symbol_address, new_symbol_source, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                source_register_num = 0;
                encodeRegisterBonusWord(operand_source, operand_destination, final_line_encode, current_are_encode, source_register_num, dest_register_num, operand_encode_2, operand_encode, new_binary_code, binary_code_table_head);
            }

            else if (new_symbol_dest)
            {
                /* Dest operand is a symbol */

                current_dest_addressing_encode = ADDRESSING3;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeSymbolBonusWord(operand_source, final_line_encode, current_are_encode, source_symbol_address, new_symbol_source, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeSymbolBonusWord(operand_destination, final_line_encode, current_are_encode, dest_symbol_address, new_symbol_dest, operand_encode, new_binary_code, binary_code_table_head);
            }

            else if (findExtern(extern_table_head, operand_destination))
            {
                /* Dest operand is an extern symbol */
                printf("Memory of extern: %d\n", *current_memory);
                /* Add to variable table as extern */
                new_variable = createVariable(operand_destination, MEMORY_START + *ic, "ext");
                addVariable(variable_table_head, new_variable);

                current_dest_addressing_encode = ADDRESSING3;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeSymbolBonusWord(operand_source, final_line_encode, current_are_encode, source_symbol_address, new_symbol_source, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeExternBonusWord(operand_destination, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
            }

            else
            {
                /* Destination operand is not int/register/entry symbol or extern symbol. */
                *passed_second = 0;
                fprintf(stdout, "Error! in line: %d, Destination operand is not known.\n", *line_number);
            }
        }
        else if (findExtern(extern_table_head, operand_source))
        {
            /* Source operand is extern symbol */

            /* Add to variable table as extern */
            new_variable = createVariable(operand_source, MEMORY_START + *ic, "ext");
            addVariable(variable_table_head, new_variable);

            current_are_encode = ABSOLUTE_ARE;
            current_source_addressing_encode = ADDRESSING3;

            /* Check dest operand type */
            if (isIntegerInRange(operand_destination, INT_INSTRUCTION_MIN_RANGE, INT_INSTRUCTION_MAX_RANGE))
            {
                /* Dest operand is int */
                current_dest_addressing_encode = ADDRESSING1;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeExternBonusWord(operand_source, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeIntBonusWord(operand_destination, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
            }
            else if (isRegisterName(operand_destination))
            {
                /* Dest operand is register */
                current_dest_addressing_encode = ADDRESSING5;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeExternBonusWord(operand_source, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                source_register_num = 0;
                encodeRegisterBonusWord(operand_source, operand_destination, final_line_encode, current_are_encode, source_register_num, dest_register_num, operand_encode_2, operand_encode, new_binary_code, binary_code_table_head);
            }
            else if (new_symbol_dest)
            {
                /* Dest operand is symbol */
                current_dest_addressing_encode = ADDRESSING3;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeExternBonusWord(operand_source, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeSymbolBonusWord(operand_destination, final_line_encode, current_are_encode, dest_symbol_address, new_symbol_dest, operand_encode, new_binary_code, binary_code_table_head);
            }
            else if (findExtern(extern_table_head, operand_destination))
            {
                /* Dest operand is extern */

                /* Add to variable table as extern */
                new_variable = createVariable(operand_destination, MEMORY_START + *ic, "ext");
                addVariable(variable_table_head, new_variable);

                current_dest_addressing_encode = ADDRESSING3;
                combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
                new_binary_code = createBinary(final_line_encode, "ins");
                addBinary(binary_code_table_head, new_binary_code);

                /* Calculating 2 extra words because of source and destination addressing */

                /* Source word */
                strcpy(operand_source, words[1]);
                encodeExternBonusWord(operand_source, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);

                /* Destination word */
                strcpy(operand_destination, words[2]);
                encodeExternBonusWord(operand_destination, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
            }
            else
            {
                /* Destination operand is not int/register/entry symbol or extern symbol. */
                *passed_second = 0;
                fprintf(stdout, "Error! in line: %d, Destination operand is not known.\n", *line_number);
            }
        }
        else
        {
            /* Source operand is not int/register/entry symbol or extern symbol. */
            *passed_second = 0;
            fprintf(stdout, "Error! in line: %d, Source operand is not known.\n", *line_number);
        }

        *ic += 2;
        *current_memory += 2;
        *line_number += 2;
    }
    else if (operands_num == 1)
    {
        strcpy(operand_destination, words[1]);

        new_symbol_dest = findSymbol(symbol_table_head, operand_destination);
        if (isIntegerInRange(operand_destination, INT_INSTRUCTION_MIN_RANGE, INT_INSTRUCTION_MAX_RANGE))
        {
            current_are_encode = ABSOLUTE_ARE;
            current_source_addressing_encode = ADDRESSING_EMPTY;
            current_dest_addressing_encode = ADDRESSING1;
            combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
            new_binary_code = createBinary(final_line_encode, "ins");
            addBinary(binary_code_table_head, new_binary_code);

            /* Calculating extra word because of destination addressing */
            strcpy(operand_destination, words[1]);
            encodeIntBonusWord(operand_destination, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
        }

        else if (isRegisterName(operand_destination))
        {
            current_are_encode = ABSOLUTE_ARE;
            current_source_addressing_encode = ADDRESSING_EMPTY;
            current_dest_addressing_encode = ADDRESSING5;
            combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
            new_binary_code = createBinary(final_line_encode, "ins");
            addBinary(binary_code_table_head, new_binary_code);

            /* Calculating extra word because of destination addressing */

            strcpy(operand_destination, words[1]);
            source_register_num = 0;
            encodeRegisterBonusWord(operand_source, operand_destination, final_line_encode, current_are_encode, source_register_num, dest_register_num, operand_encode_2, operand_encode, new_binary_code, binary_code_table_head);
        }
        else if (new_symbol_dest)
        {
            /* Operand is a known symbol */

            current_are_encode = ABSOLUTE_ARE;
            current_source_addressing_encode = ADDRESSING_EMPTY;
            current_dest_addressing_encode = ADDRESSING3;
            combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
            new_binary_code = createBinary(final_line_encode, "ins");
            addBinary(binary_code_table_head, new_binary_code);

            /* Calculating extra word because of destination addressing */

            strcpy(operand_destination, words[1]);
            encodeSymbolBonusWord(operand_destination, final_line_encode, current_are_encode, dest_symbol_address, new_symbol_dest, operand_encode, new_binary_code, binary_code_table_head);
        }
        else if (findExtern(extern_table_head, operand_destination))
        {
            /* Operand is a known extern symbol */

            /* Add to variable table as extern */
            new_variable = createVariable(operand_destination, MEMORY_START + *ic, "ext");
            addVariable(variable_table_head, new_variable);

            current_are_encode = ABSOLUTE_ARE;
            current_source_addressing_encode = ADDRESSING_EMPTY;
            current_dest_addressing_encode = ADDRESSING3;
            combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
            new_binary_code = createBinary(final_line_encode, "ins");
            addBinary(binary_code_table_head, new_binary_code);

            /* Calculating extra word because of destination addressing */

            strcpy(operand_destination, words[1]);
            encodeExternBonusWord(operand_destination, final_line_encode, current_are_encode, operand_encode, new_binary_code, binary_code_table_head);
        }
        else
        {
            /* Operand is not int/register/entry symbol or extern symbol. */
            *passed_second = 0;
            fprintf(stdout, "Error! in line: %d, Operand is not known.\n", *line_number);
        }
        *ic += 1;
        *current_memory += 1;
        *line_number += 1;
    }
    else if (operands_num == 0)
    {
        current_are_encode = ABSOLUTE_ARE;
        current_source_addressing_encode = ADDRESSING_EMPTY;
        current_dest_addressing_encode = ADDRESSING_EMPTY;
        combine4Strings(final_line_encode, current_are_encode, current_dest_addressing_encode, current_instruction_encode, current_source_addressing_encode);
        new_binary_code = createBinary(final_line_encode, "ins");
        addBinary(binary_code_table_head, new_binary_code);
    }

    /* ADD ALL THE FREE THAT NEEDED */
    free(current_instruction_encode);

    /* free(current_are_encode); 
    
    free(current_source_addressing_encode);
    
    free(current_dest_addressing_encode);*/
    free(operand_encode);
    free(operand_encode_2);
}

/* add all strings to result to complete a binary word */
void combine4Strings(char *result, const char *are, const char *dest_addressing, const char *instruction_code, const char *source_addressing)
{
    strcat(result, source_addressing);
    strcat(result, instruction_code);
    strcat(result, dest_addressing);
    strcat(result, are);
}

/* For 2 operands (2 registers) */
void combine3Strings(char *result, const char *are, const char *operand_destination, const char *operand_source)
{
    strcat(result, operand_destination);
    strcat(result, operand_source);
    strcat(result, are);
}

/* For 1 operand */
void combine2Strings(char *result, const char *are, const char *operand)
{

    strcat(result, operand);
    strcat(result, are);
}
