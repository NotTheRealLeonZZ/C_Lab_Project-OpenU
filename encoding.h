#ifndef ENCODING_H
#define ENCODING_H
#include "binary.h"
#include "symbol.h"

typedef unsigned short uint16_t;

/*
Convert a 12-bit binary number to a Base64 encoded string.

@param binaryData The binary number to be encoded.
@return The Base64 encoded string.
*/
char *convertToBase64(uint16_t binaryData);

/*
Encode a string representation of an integer to a specified length binary representation.

@param word The string representation of the integer.
@param length The desired length of the binary representation.
@return The binary representation of the integer.
*/
char *encodeStrIntToBinary(const char *word, int length);

/*
Encode a character to its 12-bit binary representation.

@param c The character to be encoded.
@return The binary representation of the character.
 */
char *encodeCharToBinary(char c);

/*
Encode an integer to its specified length binary representation.

@param num The integer to be encoded.
@param length The desired length of the binary representation.
@return The binary representation of the integer.
*/
char *encodeIntToBinary(int num, int length);

/* 
Encode a bonus word for an extern operand.

@param operand The operand value.
@param final_line_encode The final encoded line.
@param current_are_encode The current ARE value.
@param operand_encode The encoded operand value.
@param new_binary_code The new binary code node to add to the table.
@param binary_code_table_head The head of the binary code table.
*/
void encodeExternBonusWord(char operand[], char final_line_encode[], char *current_are_encode, char *operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head);

/*
Encode a bonus word for an integer operand.

@param operand The operand value.
@param final_line_encode The final encoded line.
@param current_are_encode The current ARE value.
@param operand_encode The encoded operand value.
@param new_binary_code The new binary code node to add to the table.
@param binary_code_table_head The head of the binary code table.
*/
void encodeIntBonusWord(char operand[], char final_line_encode[], char *current_are_encode, char *operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head);

/* 
Encode a bonus word for a symbol operand.

@param operand The operand value.
@param final_line_encode The final encoded line.
@param current_are_encode The current ARE value.
@param symbol_address The address of the symbol.
@param new_symbol The new symbol node.
@param operand_encode The encoded operand value.
@param new_binary_code The new binary code node to add to the table.
@param binary_code_table_head The head of the binary code table.
*/
void encodeSymbolBonusWord(char operand[], char final_line_encode[], char *current_are_encode, int symbol_address, struct Symbol *new_symbol, char *operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head);

/*
Encode a bonus word for register operands.

@param source_operand The source operand value.
@param dest_operand The destination operand value.
@param final_line_encode The final encoded line.
@param current_are_encode The current ARE value.
@param source_register_num The source register number.
@param dest_register_num The destination register number.
@param source_operand_encode The encoded source operand value.
@param dest_operand_encode The encoded destination operand value.
@param new_binary_code The new binary code node to add to the table.
@param binary_code_table_head The head of the binary code table.
*/
void encodeRegisterBonusWord(char source_operand[], char dest_operand[], char final_line_encode[], char *current_are_encode, int source_register_num, int dest_register_num, char *source_operand_encode, char *dest_operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head);

/* 
Convert a binary string to a uint16_t value.

@param binary The binary string to be converted.
@return The converted uint16_t value.
*/
uint16_t binaryStringToUInt16(const char *binary);

/* 
Write the encoded program to an output file.

@param ob_file The output file.
@param head The head of the binary code table.
@param ic The instructions counter.
@param dc The directives counter.
*/
void writeEncodedProgramToFile(FILE *ob_file, struct Binary *head, int *ic, int *dc);

#endif /* ENCODING_H */