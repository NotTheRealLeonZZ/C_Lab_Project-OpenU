#ifndef MACRO_H
#define MACRO_H

#include <stdbool.h>
#include "globals.h"

/* Structure to represent a macro */
struct Macro
{
    const char *name;
    const char *data;
    struct Macro *next;
};

/* Function to initialize the macro table */
struct Macro *createMacro(const char *macro_name, const char *macro_data);

/* Function to add a macro to the linked list */
void addMacro(struct Macro *current_macro, struct Macro *new_macro);

/* Function to find a macro by name */
struct Macro *findMacro(struct Macro *head, char words[][MAX_LINE_LENGTH], int numWords);

/* Function to copy the whole macro between 2 macro pointers */
void copyMacro(struct Macro *src, struct Macro *dst);

/* Function to print the macro table */
void printMacroTable(struct Macro *head);

/* Function to write the given macro to file, until the macro name is changed (macro finished) */
void writeMacroToOutput(struct Macro *head, FILE *outputFile);

/* Function to free memory used by the macro table */
void freeMacroTable(struct Macro **head);

#endif /* MACRO_H */
