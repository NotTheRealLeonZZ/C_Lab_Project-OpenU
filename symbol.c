#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "symbol.h"
#include "my_string.h"

/* Create a new Symbol structure */
struct Symbol *createSymbol(const char *symbol_name, const int symbol_line_num, const char *symbol_type)
{

    struct Symbol *new_symbol = (struct Symbol *)malloc(sizeof(struct Symbol));
    if (new_symbol == NULL)
    {
        fprintf(stderr, "Memory allocation failed. Exiting...\n");
        exit(1);
    }

    new_symbol->name = my_strdup(symbol_name);
    new_symbol->line_num = symbol_line_num;
    new_symbol->type = my_strdup(symbol_type);
    new_symbol->next = NULL;

    return new_symbol;
}

/* Add a symbol at the end of the node list */
void addSymbol(struct Symbol *current_symbol, struct Symbol *new_symbol)
{
    /* Assumming head is already exists */

    /* Checking if current symbol is NULL */
    if (current_symbol == NULL)
    {
        fprintf(stderr, "The given current node cannot be NULL.\n");
        return;
    }

    /* Find last symbol and change symbol's next to point to the new symbol */
    while (current_symbol->next != NULL)
    {
        current_symbol = current_symbol->next;
    }

    current_symbol->next = new_symbol;
}


/* Function to find a symbol by name.
@param head - the first pointer to the symbol table
@param name - the symbol's name you looking for
@return pointer for the found symbol or NULL */
struct Symbol *findSymbol(struct Symbol *head, const char *name)
{
    /* Checking if head of macro table is NULL */
    if (head == NULL)
    {
        fprintf(stderr, "Cannot find symbol, empty table.");
        return NULL;
    }

    struct Symbol *head_copy = head;

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


/* Print the symbol table */
void printSymbolTable(struct Symbol *head)
{
    struct Symbol *temp = head;
    printf("Symbol Table:\n");
    while (temp != NULL)
    {
        printf("\"%s\": line %d, \"%s\" -> ", temp->name, temp->line_num, temp->type);
        temp = temp->next;
        printf("\n");
    }
    printf("=================\n");
}

/* Free the memory allocated by the symbol table */
void freeSymbolTable(struct Symbol **head)
{
    struct Symbol *current = *head;
    struct Symbol *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp); /* Free the memory allocated for the node */
    }

    *head = NULL; /* Set the head pointer to NULL to indicate an empty list */
}