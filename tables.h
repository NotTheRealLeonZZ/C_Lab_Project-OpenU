#ifndef TABLES_H
#define TABLES_H

#include <stdbool.h>
#include "symbol.h"
#include "extern.h"

bool tablesAreDifferent(struct Symbol *symbol_table, struct Extern *extern_table);

#endif /* BINARY_H */