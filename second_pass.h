#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "symbol.h"
#include "extern.h"

/* Function to perform the pre-assembly tasks */
void secondPass(const char *input_file, struct Symbol *symbol_table, struct Extern *extern_table, int passed_first);

#endif /* SECOND_PASS_H */