#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include "symbol.h"
#include "extern.h"

/* Function to perform the pre-assembly tasks */
void firstPass(const char *input_file, struct Symbol *symbol_table, struct Extern *extern_table, int *passed_first, int *is_ent);

#endif /* FIRST_PASS_H */