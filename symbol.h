#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

#define MAX_SYMBOL_NAME_LENGTH 31
#define INSTRUCTION "ins"
#define DIRECTIVE "dir"

/* Structure to represent a symbol */
struct Symbol
{
    const char *name;
    int line_num;
    const char *type; /* instruction or directive */
    struct Symbol *next;
};


/* Create a new Symbol structure */
struct Symbol *createSymbol(const char *symbol_name, const int symbol_line_num, const char *symbol_type);

/* Add a symbol at the end of the node list */
void addSymbol(struct Symbol *current_symbol, struct Symbol *new_symbol);

/* Function to find a symbol by name.
@param head - the first pointer to the symbol table
@param name - the symbol's name you looking for
@return pointer for the found symbol or NULL */
struct Symbol *findSymbol(struct Symbol *head, const char *name);

/* Print the symbol table */
void printSymbolTable(struct Symbol *head);

/* Free the memory allocated by the symbol table */
void freeSymbolTable(struct Symbol **head);

#endif /* SYMBOL_H */