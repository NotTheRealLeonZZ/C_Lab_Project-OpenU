#ifndef ENCODING_H
#define ENCODING_H
#include <stdint.h>
#include "binary.h"
#include "symbol.h"

char *convertToBase64(uint16_t binaryData);

char *encodeStrIntToBinary(const char *word, int length);

char *encodeCharToBinary(char c);

char *encodeIntToBinary(int num, int length);

void encodeExternBonusWord(char operand[], char final_line_encode[], char *current_are_encode, char *operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head);

void encodeIntBonusWord(char operand[], char final_line_encode[], char *current_are_encode, char *operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head);

void encodeSymbolBonusWord(char operand[], char final_line_encode[], char *current_are_encode, int symbol_address, struct Symbol *new_symbol, char *operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head);

void encodeRegisterBonusWord(char source_operand[], char dest_operand[], char final_line_encode[], char *current_are_encode, int source_register_num, int dest_register_num, char *source_operand_encode, char *dest_operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head);

#endif /* ENCODING_H */