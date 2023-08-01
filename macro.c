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

/* Add a macro at the end of the node list */
void addMacro(struct Macro *current_macro, struct Macro *new_macro)
{
    /* Assumming head is already exists */

    /* Checking if current macro is NULL */
    if (current_macro == NULL)
    {
        fprintf(stderr, "The given current node cannot be NULL.\n");
        return;
    }

    /* Find last macro and change macro's next to point to the new macro */
    while (current_macro->next != NULL)
    {
        current_macro = current_macro->next;
    }

    current_macro->next = new_macro;
}

/* Probably delete this */
void copyMacro(struct Macro *src, struct Macro *dst)
{
    dst->name = src->name;
    dst->data = src->data;
    dst->next = src->next;
}

/* Function to find a macro by name.
@param head - the first pointer to the macro table
@param words - edit
@param numWords - edit
@return pointer for the found macro or NULL */
struct Macro *findMacro(struct Macro *head, char words[][MAX_LINE_LENGTH], int numWords)
{
    struct Macro *head_copy;
    char current_name[MAX_LINE_LENGTH];
    int i;

    /* Checking if head of macro table is NULL */
    if (head == NULL)
    {
        return NULL;
    }

    for (i = 0; i < numWords; i++)
    {
        head_copy = head;
        while (head_copy != NULL)
        {
            strcpy(current_name, head_copy->name);
            if (strcmp(current_name, words[i]) == 0)
            {
                return head_copy;
            }

            else
            {
                while ((head_copy->next != NULL) && (strcmp(head_copy->next->name, current_name) == 0))
                {
                    head_copy = head_copy->next;
                }
            }
            head_copy = head_copy->next;
        }
    }
    return NULL;
}

void printMacroTable(struct Macro *head)
{
    printf("Macro Table:\n");
    struct Macro *temp = head;
    while (temp != NULL)
    {
        printf("\"%s\": \"%s\" -> ", temp->name, temp->data);
        temp = temp->next;
        printf("\n");
    }
    printf("=================\n");
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

/* Free the memory allocated by the macro table */
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
