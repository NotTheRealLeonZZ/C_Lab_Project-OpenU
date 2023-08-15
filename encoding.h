#ifndef ENCODING_H
#define ENCODING_H
#include <stdint.h>

char *convertToBase64(uint16_t binaryData);

char *encodeStrIntToBinary(const char *word, int length);

char *encodeCharToBinary(char c);

char *encodeIntToBinary(int num, int length);

#endif /* ENCODING_H */