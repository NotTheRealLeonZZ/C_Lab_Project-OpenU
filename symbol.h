#ifndef SYMBOL_H
#define SYMBOL_H

#include "globals.h"

/* A symbol must be seperated from the next command with a space! */

/* Structure to represent a symbol */
struct Symbol
{
    const char *name;    /* Name of symbol */
    int address;         /* Address of symbol */
    const char *type;    /* instruction or directive */
    struct Symbol *next; /* Next pointer to next symbol */
};

/*
Create a new symbol structure.

@param symbol_name The name of the symbol.
@param symbol_address The address associated with the symbol.
@param symbol_type The type of the symbol (instruction or directive).
@return Pointer to the newly created symbol structure.
*/
struct Symbol *createSymbol(const char *symbol_name, const int symbol_address, const char *symbol_type);

/*
Add a symbol to the end of the symbol table.

@param current_symbol Pointer to the current symbol node.
@param new_symbol Pointer to the new symbol to be added.
*/
void addSymbol(struct Symbol *current_symbol, struct Symbol *new_symbol);

/*
Find a symbol in the symbol table by its name.

@param head Pointer to the head of the symbol table.
@param name The name of the symbol to search for.
@return Pointer to the found symbol, or NULL if not found.
*/
struct Symbol *findSymbol(struct Symbol *head, const char *name);

/*
Check if a given word can be a symbol.

@param word The word to be checked.
@return true if the word is a symbol, otherwise false.
*/
bool wordIsSymbol(char *word);

/*
Remove a symbol from a line.

@param line The line to be modified.
@param symbol_name The name of the symbol to be removed.
*/
void removeSymbolFromLine(char *line, char *symbol_name);

/*
Print a warning message if a symbol is of .entry or .extern.

@param word The word to be checked as a symbol.
@param line_number The line number where the symbol is used.
*/
void warnSymbolIfNecessary(char *word, int line_number);

/*
Update the address of a symbol to reflect its final address.

@param symbol_table_head_copy Pointer to the head of the symbol table.
@param ic The final value of the instructions counter.
*/
void fixSymbolAddress(struct Symbol *symbol_table_head_copy, int ic);

/*
Free the memory allocated by the symbol table.

@param head Pointer to the pointer to the head of the symbol table.
*/
void freeSymbolTable(struct Symbol **head);

#endif /* SYMBOL_H */