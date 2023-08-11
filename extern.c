#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "extern.h"
#include "my_string.h"
#include "globals.h"

struct Extern *createExtern(const char *extern_name)
{
    struct Extern *new_extern = (struct Extern *)malloc(sizeof(struct Extern));
    if (new_extern == NULL)
    {
        fprintf(stdout, "Memory allocation failed. Exiting...\n");
        exit(1);
    }

    new_extern->name = my_strdup(extern_name);
    new_extern->next = NULL;

    return new_extern;
}

/* Add a symbol at the end of the node list */
void addExtern(struct Extern *current_extern, struct Extern *new_extern)
{
    /* Assumming head is already exists */

    /* Checking if current extern is NULL */
    if (current_extern == NULL)
    {
        fprintf(stderr, "The given current node cannot be NULL.\n");
        return;
    }

    /* Find last symbol and change symbol's next to point to the new symbol */
    while (current_extern->next != NULL)
    {
        current_extern = current_extern->next;
    }

    current_extern->next = new_extern;
}

/* Function to find a symbol by name.
Maybe change it to find all symbols in line
@param head - the first pointer to the symbol table
@param name - the symbol's name you looking for
@return pointer for the found symbol or NULL */
struct Extern *findExtern(struct Extern *head, const char *name)
{
    /* Checking if head of macro table is NULL */
    if (head == NULL)
    {
        fprintf(stderr, "Cannot find symbol, empty table.");
        return NULL;
    }

    struct Extern *head_copy = head;

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

/* Print the extern table */
void printExternTable(struct Extern *head)
{
    struct Extern *temp = head;
    printf("Extern Table:\n");
    while (temp != NULL)
    {
        printf("\"%s\"", temp->name);
        temp = temp->next;
        printf("\n");
    }
    printf("=================\n");
}

/* Free the memory allocated by the extern table */
void freeExternTable(struct Extern **head)
{
    struct Extern *current = *head;
    struct Extern *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp); /* Free the memory allocated for the node */
    }

    *head = NULL; /* Set the head pointer to NULL to indicate an empty list */
}