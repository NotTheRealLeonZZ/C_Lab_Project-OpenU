/* Example */

#include <stdio.h>
#include <stdint.h>

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

int main()
{
    /* Example: Store 12 bits of binary data in a uint16_t */
    uint16_t binaryData = 0b101000001100;

    char *base64Str = convertToBase64(binaryData);

    /* Print the Base64-encoded data */
    printf("Base64: %s\n", base64Str);

    return 0;
}