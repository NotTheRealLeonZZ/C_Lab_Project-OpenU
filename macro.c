/*

This file is all about macros and macro tables.
It contains all the function I will need for the pre-assembler part.
Also everything that handles memory for these purposes.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "my_string.h"

struct Macro *createMacro(const char *macro_name, const char *macro_data)
{

    struct Macro *new_macro = (struct Macro *)malloc(sizeof(struct Macro));
    if (new_macro == NULL)
    {
        fprintf(stderr, "Memory allocation failed. Exiting...\n");
        exit(1);
    }

    new_macro->name = my_strdup(macro_name);
    new_macro->data = my_strdup(macro_data);
    new_macro->next = NULL;

    return new_macro;
}

void addMacro(struct Macro *current_macro, struct Macro *new_macro)
{
    /* Assumming head is already exists */

    /* Checking if current macro is NULL */
    if (current_macro == NULL)
    {
        fprintf(stderr, "The given current node cannot be NULL.\n");
        return;
    }

    /* Changing current macro's next to point to the new macro */
    while (current_macro->next != NULL)
    {
        current_macro = current_macro->next;
    }

    current_macro->next = new_macro;
}

void copyMacro(struct Macro *src, struct Macro *dst)
{
    dst->name = src->name;
    dst->data = src->data;
    dst->next = src->next;
}

/* Function to find a macro by name.
@param head - the first pointer to the macro table
@param name - the macro's name you looking for
@return pointer for the found macro or NULL */
struct Macro *findMacro(struct Macro *head, const char *name)
{
    /* Checking if head of macro table is NULL */
    if (head == NULL)
    {
        fprintf(stderr, "Cannot find macro, empty table.");
        return NULL;
    }

    struct Macro *head_copy = head;

    while (head_copy != NULL)
    {
        if (strcmp(head_copy->name, name) == 0)
        {
            return head_copy;
        }
        head_copy = head_copy->next;
    }
    return NULL;
}

void printMacroTable(struct Macro *head)
{
    struct Macro *temp = head;
    while (temp != NULL)
    {
        printf("\"%s\": \"%s\" -> ", temp->name, temp->data);
        temp = temp->next;
        printf("\n");
    }
}

void writeMacroToOutput(struct Macro *head, FILE *outputFile)
{
    char *name = head->name;
    while (strcmp(head->name, name) == 0)
    {
        fputs(head->data, outputFile);
        head = head->next;
        if (head == NULL)
        {
            break;
        }
    }
}

void freeMacroTable(struct Macro **head)
{
    struct Macro *current = *head;
    struct Macro *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp); /* Free the memory allocated for the node */
    }

    *head = NULL; /* Set the head pointer to NULL to indicate an empty list */
}
