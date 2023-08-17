#include <stdio.h>

#include <string.h>
#include "symbol.h"
#include "extern.h"
#include "globals.h"

bool tablesAreDifferent(struct Symbol *symbol_table, struct Extern *extern_table)
{
    struct Symbol *symbol_table_copy = symbol_table;
    struct Extern *extern_table_copy = extern_table;
    bool found = false;

    while (symbol_table_copy != NULL)
    {
        extern_table_copy = extern_table;
        /* Check current symbol with every extern */
        while (extern_table_copy != NULL)
        {
            if (strcmp(symbol_table_copy->name, extern_table_copy->name) == 0)
            {
                found = true;
                break;
            }
            extern_table_copy = extern_table_copy->next;
        }

        if (found)
        {
            return false; /* Tables are not different */
        }
        symbol_table_copy = symbol_table_copy->next;
    }
    return true;
}