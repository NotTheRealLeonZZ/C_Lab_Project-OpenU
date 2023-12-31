#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "my_string.h"
#include "globals.h"

/* Initialize a new macro. */
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

/* Add a macro to the linked list. */
void addMacro(struct Macro *current_macro, struct Macro *new_macro)
{
    /* Assumming head is already exists */

    /* Checking if current macro is NULL (memory error) */
    if (current_macro == NULL)
    {
        fprintf(stdout, "The given current node cannot be NULL.\n");
        return;
    }

    /* Find last macro and change macro's next to point to the new macro */
    while (current_macro->next != NULL)
    {
        current_macro = current_macro->next;
    }

    current_macro->next = new_macro;
}

/* Find a macro by its name. */
struct Macro *findMacro(struct Macro *head, char words[][MAX_LINE_LENGTH], int numWords)
{
    struct Macro *head_copy;
    char current_name[MAX_LINE_LENGTH];
    int i;

    /* Checking if head of macro table is NULL */
    if (head == NULL)
    {
        fprintf(stdout, "Cannot find macro, empty table.");
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

/* Write the data of a macro to a file until the macro name changes (macro finished). */
void writeMacroToOutput(struct Macro *head, FILE *outputFile)
{
    char name[MAX_MACRO_NAME];
    strcpy(name, head->name);
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

/* Free the memory used by the macro table. */
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
