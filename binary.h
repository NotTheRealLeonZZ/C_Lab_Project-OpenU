#ifndef BINARY_H
#define BINARY_H

#include "globals.h"

/*
Binary table holds every line encoded to binary, accourding to the instructions in course book.
Each word is 12 bit binary. and gets a instruction or directive type for later encode to base64.

Encode includes:
1) For instructions:
Source addressing method -> instruction code -> Destination addressing method -> A.R.E
and adds another memory word accouring to source & destination addressing method.
2) For directives:
If its .data, each int gets its own memory word, encoded to 12bit binary
If its .string, each char gets its own memory word, encoded to 12 bit binary + '\0'
*/

/* Structure to represent a binary code */
struct Binary
{
    const char *code;
    const char *type; /* instruction or directive */
    struct Binary *next;
};

/*
Create a new Binary node

@param *binary_code The binary string
@param *type The type of the binary code
@return New binary node.
*/
struct Binary *createBinary(const char *binary_code, const char *type);

/*
Add a binary code at the end of the node list 

@param current_binary_code The current last binary code node.
@param new_binary_code The new binary code node to add.
*/
void addBinary(struct Binary *current_binary_code, struct Binary *new_binary_code);

/*
Print the binary table 

@param head The head of the binary table.
*/
void printBinaryTable(struct Binary *head);

/*
Free the memory allocated by the binary table 

@param head A pointer to the head of the binary table.
*/
void freeBinaryTable(struct Binary **head);

#endif /* BINARY_H */