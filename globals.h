#ifndef GLOBALS_H
#define GLOBALS_H

typedef enum
{
    false = 0,
    true = 1
} bool;

#define MAX_PROGRAM_SIZE 923
#define MAX_MACRO_NAME 76

#define MAX_LINE_LENGTH 82       /*  Maximum line length, including '\n' and '\0' */
#define MAX_INSTRUCTION_LENGTH 5 /* Maximum instruction string length */
#define MEMORY_START 100

#define NUM_OF_REGISTERS 8

#define MAX_SYMBOL_NAME_LENGTH 32 /* including \0 */
#define SYMBOL_TYPE_LENGTH 4
#define INSTRUCTION "ins"
#define DIRECTIVE "dir"

#define NUM_OF_DIRECTIVES 4
#define NUM_OF_CHARACTERS_FOR_DIRECTIVE 8

#define NUM_OF_INSTRUCTIONS 16

#define MAX_FILE_NAME_LENGTH 100

#define MAX_WORDS_IN_A_LINE 40

#define NUM_OF_ADDRESSING_METHODS 3

#define BINARY_CODE_LENGTH 12
#define BINARY_CODE_WITH_ARE 10
#define BINARY_CODE_LENGTH_WITH_HEADER 14
#define BINARY_CODE_FOR_REGISTER_OPERANDS_SIZE 5

#define ADDRESSING1 "001"
#define ADDRESSING3 "011"
#define ADDRESSING5 "101"
#define ADDRESSING_EMPTY "000"

#define ABSOLUTE_ARE "00"
#define EXTERNAL_ARE "01"
#define RELOCATABLE_ARE "10"

#define SIZE_OF_ARE 2
#define SIZE_OF_ADDRESSING_METHOD 3
#define SIZE_OF_INSTRUCTION_CODE 4

#define INT_INSTRUCTION_MIN_RANGE -512
#define INT_INSTRUCTION_MAX_RANGE 511
#define INT_DIRECTIVE_MIN_RANGE -2048
#define INT_DIRECTIVE_MAX_RANGE 2047

#define BINARY_REPRESENTATION_SIZE 17

#endif /* GLOBALS_H */