#ifndef MACRO_H
#define MACRO_H

#include "globals.h"

/*

This file is all about macros and macro tables.
It contains all the function I will need for the pre-assembler part.
Also everything that handles memory for these purposes.

*/

/* Structure to represent a macro 
Macro has name, data and next pointer to next macro.
If a macro is multi-line, each line will have its own node.
When outputting the macros to the .am file it continues to print as fat as the name is the same.
*/
struct Macro
{
    const char *name;   /* Macro name */
    const char *data;   /* Macro data */
    struct Macro *next; /* Next pointer */
};

/*
Initialize a new macro.

@param macro_name The name of the macro.
@param macro_data The data of the macro.
@return A pointer to the newly created macro.
*/
struct Macro *createMacro(const char *macro_name, const char *macro_data);

/*
Add a macro to the linked list.

@param current_macro Pointer to the current macro in the linked list.
@param new_macro Pointer to the new macro to be added.
*/
void addMacro(struct Macro *current_macro, struct Macro *new_macro);

/*
Find a macro by its name.

@param head Pointer to the head of the macro linked list.
@param words The words in the line.
@param numWords The number of words in the line.
@return A pointer to the found macro or NULL if not found.
*/
struct Macro *findMacro(struct Macro *head, char words[][MAX_LINE_LENGTH], int numWords);

/*
Print the contents of the macro table.

@param head Pointer to the head of the macro linked list.
*/
void printMacroTable(struct Macro *head);

/*
Write the data of a macro to a file until the macro name changes (macro finished).

@param head Pointer to the head of the macro linked list.
@param outputFile Pointer to the output file where the macro data will be written.
*/
void writeMacroToOutput(struct Macro *head, FILE *outputFile);

/*
Free the memory used by the macro table.

@param head Pointer to a pointer to the head of the macro linked list.
*/
void freeMacroTable(struct Macro **head);

#endif /* MACRO_H */
