/* Example */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "my_string.h"
#include "instructions.h"
#include "binary.h"
#include "symbol.h"
#include "registers.h"

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *convertToBase64(uint16_t binaryData)
{
    static char base64String[2];
    binaryData &= 0x0FFF; /* Ensure we only consider the lower 12 bits */

    /* Base64 encoding requires 6 bits per character */
    base64String[0] = base64_table[(binaryData >> 6) & 0x3F];
    base64String[1] = base64_table[binaryData & 0x3F];
    base64String[2] = '\0'; /* Null-terminate the string */

    return base64String;
}

char *encodeStrIntToBinary(const char *word, int length)
{
    long num = strtol(word, NULL, 10); /* Convert decimal string to long */

    /* Calculate the maximum positive value for the given length */
    long maxPositive = (1 << (length - 1)) - 1;
    /* Calculate the minimum negative value for the given length */
    long minNegative = -(1 << (length - 1));

    /* Check if the number is within the representable range */
    if (num < minNegative || num > maxPositive)
    {
        return NULL; /* Number is out of range */
    }

    /* Handle negative numbers */
    if (num < 0)
    {
        num = (1 << length) + num; /* Convert to 2's complement */
    }

    static char binary[17]; /* Assuming 16-bit binary representation */
    int i = 15;

    /* Convert decimal to binary */
    while (num > 0 && i >= 0)
    {
        binary[i--] = num % 2 + '0';
        num /= 2;
    }

    /* Fill the remaining bits with 0 */
    while (i >= 0)
    {
        binary[i--] = '0';
    }

    binary[16] = '\0'; /* Null-terminate the string */
    return my_strdup(&binary[16 - length]);
}

char *encodeCharToBinary(char c)
{
    int asciiValue = (int)c;
    int i;

    char *binary = (char *)malloc(13 * sizeof(char)); /* 12 bits + null terminator */
    if (binary == NULL)
    {
        fprintf(stderr, "Memory allocation failed. Exiting...\n");
        exit(1);
    }

    for (i = 11; i >= 0; i--)
    {
        binary[11 - i] = (asciiValue & (1 << i)) ? '1' : '0';
    }
    binary[12] = '\0'; /* Null-terminate the binary string */

    return binary;
}

char *encodeIntToBinary(int original_num, int length)
{
    char *binary = (char *)malloc(length + 1); /* +1 for the null terminator */
    int i;
    if (binary == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    /* Handle negative numbers using 2's complement */
    if (original_num < 0)
    {
        original_num = (1 << length) + original_num;
    }

    for (i = length - 1; i >= 0; i--)
    {
        binary[i] = (original_num & 1) + '0';
        original_num >>= 1;
    }
    binary[length] = '\0';

    return binary;
}

void encodeExternBonusWord(char operand[], char final_line_encode[], char *current_are_encode, char *operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head)
{

    final_line_encode[0] = '\0';
    current_are_encode = EXTERNAL_ARE;
    operand_encode = encodeIntToBinary(0, BINARY_CODE_WITH_ARE);
    combine2Strings(final_line_encode, current_are_encode, operand_encode);
    new_binary_code = createBinary(final_line_encode, "ins");
    addBinary(binary_code_table_head, new_binary_code);
    printf("final line for operand: %s\n", final_line_encode);
}

void encodeIntBonusWord(char operand[], char final_line_encode[], char *current_are_encode, char *operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head)
{

    final_line_encode[0] = '\0';
    operand_encode = encodeStrIntToBinary(operand, BINARY_CODE_WITH_ARE);
    combine2Strings(final_line_encode, current_are_encode, operand_encode);
    new_binary_code = createBinary(final_line_encode, "ins");
    addBinary(binary_code_table_head, new_binary_code);
    printf("final line for operand: %s\n", final_line_encode);
}

void encodeSymbolBonusWord(char operand[], char final_line_encode[], char *current_are_encode, int symbol_address, struct Symbol *new_symbol, char *operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head)
{
    final_line_encode[0] = '\0';
    current_are_encode = RELOCATABLE_ARE;
    symbol_address = new_symbol->address;
    operand_encode = encodeIntToBinary(symbol_address, BINARY_CODE_WITH_ARE);
    combine2Strings(final_line_encode, current_are_encode, operand_encode);
    new_binary_code = createBinary(final_line_encode, "ins");
    addBinary(binary_code_table_head, new_binary_code);
    printf("final line for operand: %s\n", final_line_encode);
}

void encodeRegisterBonusWord(char source_operand[], char dest_operand[], char final_line_encode[], char *current_are_encode, int source_register_num, int dest_register_num, char *source_operand_encode, char *dest_operand_encode, struct Binary *new_binary_code, struct Binary *binary_code_table_head)
{
    final_line_encode[0] = '\0';
    dest_register_num = getRegisterNum(dest_operand);
    dest_operand_encode = encodeIntToBinary(dest_register_num, BINARY_CODE_FOR_REGISTER_OPERANDS_SIZE);
    source_operand_encode = encodeIntToBinary(source_register_num, BINARY_CODE_FOR_REGISTER_OPERANDS_SIZE);
    combine3Strings(final_line_encode, current_are_encode, source_operand_encode, dest_operand_encode);
    new_binary_code = createBinary(final_line_encode, "ins");
    addBinary(binary_code_table_head, new_binary_code);
    printf("final line for operand: %s\n", final_line_encode);
}
/* int main()
{
    /* Example: Store 12 bits of binary data in a uint16_t 
uint16_t binaryData = 0b101000001100;

char *base64Str = convertToBase64(binaryData);

/* Print the Base64-encoded data 
printf("Base64: %s\n", base64Str);

return 0;
}
*/