#ifndef BINARY_H
#define BINARY_H

#include <stdbool.h>
#include "globals.h"

/* Structure to represent a symbol */
struct Binary
{
    const char *code;
    struct Binary *next;
};

/* Create a new Binary structure */
struct Binary *createBinary(const char *binary_code);

/* Add a binary code at the end of the node list */
void addBinary(struct Binary *current_binary_code, struct Binary *new_binary_code);

/* Find binary in binary table */
struct Binary *findBinary(struct Binary *head, const char *code);

/* Print the binary table */
void printBinaryTable(struct Binary *head);

/* Free the memory allocated by the binary table */
void freeBinaryTable(struct Binary **head);

#endif /* BINARY_H */