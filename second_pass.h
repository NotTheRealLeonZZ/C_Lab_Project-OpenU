#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "symbol.h"
#include "extern.h"
#include "binary.h"
#include "variables.h"

/*
In second pass I have a complete symbol list and syntax ready assembly file.
I continue to check if operands are correct, every .entry is in symbol list, every external symbol is decleared with .extern.
I also re-count memory,ic and dc. Encode each line to 12 bit binary and build binary table,
Build variable table of entry and extern to output later to files.
*/

/* Function to perform the second pass tasks */
void secondPass(const char *input_file, struct Symbol *symbol_table, struct Extern *extern_table, struct Binary *binary_code_table, struct Variable *variable_table, int *passed_second, int *ic, int *dc, int *is_ext);

#endif /* SECOND_PASS_H */