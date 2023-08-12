#ifndef ENCODING_H
#define ENCODING_H
#include <stdint.h>

char *convertToBase64(uint16_t binaryData);

char *encodeIntToBinary(const char *word, int length);

char *encodeCharToBinary(char c);

#endif /* ENCODING_H */