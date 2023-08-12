#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "symbol.h"
#include "extern.h"
#include "binary.h"
#include "variables.h"

/* Function to perform the pre-assembly tasks */
void secondPass(const char *input_file, struct Symbol *symbol_table, struct Extern *extern_table, struct Binary *binary_code_table, struct Variable *variable_table, int *passed_second)

#endif /* SECOND_PASS_H */