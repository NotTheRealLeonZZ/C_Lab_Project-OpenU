#ifndef MACRO_H
#define MACRO_H

#include <stdbool.h>


#define MAX_LINE_LENGTH 81

/* Structure to represent a macro */
struct Macro
{
    const char *name;
    const char *data;
    struct Macro *next;
};

/* Structure to represent a macro table 
typedef struct {
    Macro* macros;
    int count;
    int capacity;
} MacroTable; */

/* Function to initialize the macro table */
struct Macro *createMacro(const char *macro_name, const char *macro_data);

/* Function to add a macro to the linked list */
void addMacro(struct Macro *current_macro, struct Macro *new_macro);

/* Function to find a macro by name */
struct Macro *findMacro(struct Macro *head_of_macro_table, const char *name);

/* Function to copy the whole macro between 2 macro pointers */
void copyMacro(struct Macro *src, struct Macro *dst);

/* Function to print the macro table */
void printMacroTable(struct Macro *head);

/* Function to free memory used by the macro table */
void freeMacroTable(struct Macro *head);

#endif /* MACRO_H */
