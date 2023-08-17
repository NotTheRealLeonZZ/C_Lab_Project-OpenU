#ifndef TABLES_H
#define TABLES_H
#include "symbol.h"
#include "extern.h"
#include "globals.h"

/*
Check if the symbol table and extern table have distinct symbols.

@param symbol_table Pointer to the head of the symbol table.
@param extern_table Pointer to the head of the extern table.
@return true if there are no common symbols between the two tables, false otherwise.
*/
bool tablesAreDifferent(struct Symbol *symbol_table, struct Extern *extern_table);

#endif /* BINARY_H */