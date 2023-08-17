#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "globals.h"
#include "symbol.h"
#include "variables.h"
#include "extern.h"
#include "binary.h"

/*
Instruction struct will hold all instruction details:
name, number of legal operands, decimal code of instruction, source addressing methods and destination addressing methods.
Will use this to create the nodes and make all checks needed that is instruction related
*/

/* Represents an instruction with its details and addressing methods. */
struct Instruction
{
    const char *name;                                             /* Name of instruction */
    const int operands_num;                                       /* Number of legal operands */
    int decimal_code;                                             /* Decimal code of instruction */
    int source_addressing_method[NUM_OF_ADDRESSING_METHODS];      /* Source addressing methods */
    int destination_addressing_method[NUM_OF_ADDRESSING_METHODS]; /* Destination addressing methods */
};

/* Declare an array of instuctions */
extern const struct Instruction instructionsArray[NUM_OF_INSTRUCTIONS];

/* 
Get the decimal code of an instruction by its name.

@param received_name The name of the instruction.
@return The decimal code of the instruction.
*/
int get_decimal_code(char *received_name);

/*
Check if a given name corresponds to an instruction name.
 
@param received_name The name to check.
@return True if the name corresponds to an instruction name, false otherwise.
*/
bool isInstructionName(char *received_name);

/*
Validate the destination operand one at a time.

@param operand The operand to validate.
@param index The index of the instruction in the instructions array.
@param line_number The line number of the current line.
@return True if the operand is valid, false otherwise.
*/
bool validateOneOperandDest(char *operand, int index, int line_number);

/*
Validate the source operand one at a time.

@param operand The operand to validate.
@param index The index of the instruction in the instructions array.
@param line_number The line number of the current line.
@return True if the operand is valid, false otherwise.
*/
bool validateOneOperandSource(char *operand, int index, int line_number);

/*
Check for comma problems in the instruction line.

@param line The instruction line.
@param line_number The line number of the current line.
@param num_words The number of words in the line.
@param instruction_full_name The full name of the instruction.
@param index The index of the instruction in the instructions array.
@return True if comma problems are found, false otherwise.
*/
bool instructionCommaProblem(char *line, int line_number, int num_words, char *instruction_full_name, int index);

/*
Check for operand problems in the instruction line.

@param words The words in the line.
@param num_words The number of words in the line.
@param line The instruction line.
@param line_number The line number of the current line.
@param instruction_full_name The full name of the instruction.
@param index The index of the instruction in the instructions array.
@return True if operand problems are found, false otherwise.
*/
bool instructionOperandsProblem(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number,
                                char *instruction_full_name, int index);

/*
Validate instruction syntax.

@param words The words in the line.
@param num_words The number of words in the line.
@param line The instruction line.
@param line_number The line number of the current line.
@return True if instruction syntax is valid, false otherwise.
*/
bool validInstruction(char words[][MAX_LINE_LENGTH], int num_words, char *line, int line_number);

/*
Calculate the binary representation of an instruction and add it to the binary code table.

@param words The words in the line.
@param num_words The number of words in the line.
@param binary_code_table_head Pointer to the head of the binary code table.
@param symbol_table_head Pointer to the head of the symbol table.
@param variable_table_head Pointer to the head of the variable table.
@param extern_table_head Pointer to the head of the extern table.
@param line_number Pointer to the current line number.
@param passed_second Pointer to the second pass status.
@param current_memory Pointer to the current memory address.
@param ic Pointer to the instructions counter.
@param is_ext Pointer to the extern flag.
*/
void calculateInstructionBinary(char words[][MAX_LINE_LENGTH], int num_words, struct Binary *binary_code_table_head,
                                struct Symbol *symbol_table_head, struct Variable *variable_table_head, struct Extern *extern_table_head,
                                int *line_number, int *passed_second, int *current_memory, int *ic, int *is_ext);

/*
Combine four strings to create a binary word.

@param result The resulting binary word.
@param are The ARE part of the word.
@param dest_addressing The destination addressing part of the word.
@param instruction_code The instruction code part of the word.
@param source_addressing The source addressing part of the word.
*/
void combine4Strings(char *result, const char *are, const char *dest_addressing, const char *instruction_code, const char *source_addressing);

/*
Combine three strings to create a binary word.

@param result The resulting binary word.
@param are The ARE part of the word.
@param operand1 The first operand part of the word.
@param operand2 The second operand part of the word.
*/
void combine3Strings(char *result, const char *are, const char *operand1, const char *operand2);

/*
Combine two strings to create a binary word.

@param result The resulting binary word.
@param are The ARE part of the word.
@param operand The operand part of the word.
*/
void combine2Strings(char *result, const char *are, const char *operand);

#endif /* INSTRUCTIONS_H */
