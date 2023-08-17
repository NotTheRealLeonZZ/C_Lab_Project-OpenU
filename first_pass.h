#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include "symbol.h"
#include "extern.h"

/*
In first pass I parse the .am file and build the symbol table.
Symbol names must end with ':' and the next word will be seperated with space.
While parsing the file, I also calculate the memory addresses , IC and DC.
I create extern table to later check every unknown symbol if its extern.
Each symbol is validated for syntax error.
In the end, I check that the max memory isn't passed.
 */

/* Function to perform the first pass tasks */
void firstPass(const char *input_file, struct Symbol *symbol_table, struct Extern *extern_table, int *passed_first, int *is_ent);

#endif /* FIRST_PASS_H */