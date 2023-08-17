#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_string.h"
#include "variables.h"

/* Create a new Variable structure. */
struct Variable *createVariable(const char *variable_name, const int variable_address, const char *variable_type)
{
    struct Variable *new_variable = (struct Variable *)malloc(sizeof(struct Variable));
    if (new_variable == NULL)
    {
        fprintf(stdout, "Memory allocation failed. Exiting...\n");
        exit(1);
    }

    new_variable->name = my_strdup(variable_name);
    new_variable->address = variable_address;
    new_variable->type = my_strdup(variable_type);
    new_variable->next = NULL;

    return new_variable;
}

/* Add a variable at the end of the node list. */
void addVariable(struct Variable *current_variable, struct Variable *new_variable)
{
    /* Assumming head is already exists */

    /* Checking if current symbol is NULL (memory error) */
    if (current_variable == NULL)
    {
        fprintf(stderr, "The given current node cannot be NULL.\n");
        return;
    }

    /* Find last symbol and change symbol's next to point to the new symbol */
    while (current_variable->next != NULL)
    {
        current_variable = current_variable->next;
    }

    current_variable->next = new_variable;
}

/* Write the entry variables to a file. */
void writeEntVariablesToFile(FILE *ent_file, struct Variable *head)
{
    struct Variable *temp = head;

    while (temp != NULL)
    {
        if (strcmp(temp->type, "ent") == 0)
        {
            fprintf(ent_file, "%s %d\n", temp->name, temp->address);
        }
        temp = temp->next;
    }
}

/* Write the extern variables to a file. */
void writeExtVariablesToFile(FILE *ext_file, struct Variable *head)
{
    struct Variable *temp = head;

    while (temp != NULL)
    {
        if (strcmp(temp->type, "ext") == 0)
        {
            fprintf(ext_file, "%s %d\n", temp->name, temp->address);
        }

        temp = temp->next;
    }
}

/* Free the memory allocated by the variable table. */
void freeVariableTable(struct Variable **head)
{
    struct Variable *current = *head;
    struct Variable *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp); /* Free the memory allocated for the node */
    }

    *head = NULL; /* Set the head pointer to NULL to indicate an empty list */
}