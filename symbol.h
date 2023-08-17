#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

/* Structure to represent a symbol */
struct Symbol
{
    const char *name;
    int address;
    const char *type; /* instruction or directive */
    struct Symbol *next;
};

/* Create a new Symbol structure */
struct Symbol *createSymbol(const char *symbol_name, const int symbol_address, const char *symbol_type);

/* Add a symbol at the end of the node list */
void addSymbol(struct Symbol *current_symbol, struct Symbol *new_symbol);

/* Function to find a symbol by name.
@param head - the first pointer to the symbol table
@param name - the symbol's name you looking for
@return pointer for the found symbol or NULL */
struct Symbol *findSymbol(struct Symbol *head, const char *name);

bool wordIsSymbol(char *word);

void removeSymbolFromLine(char *line, char *symbol_name);

void warnSymbolIfNecessary(char *word, int line_number);

void fixSymbolAddress(struct Symbol *symbol_table_head_copy, int ic);

/* Print the symbol table */
void printSymbolTable(struct Symbol *head);

/* Free the memory allocated by the symbol table */
void freeSymbolTable(struct Symbol **head);

#endif /* SYMBOL_H */