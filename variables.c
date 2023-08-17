#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_string.h"
#include "variables.h"

/*
 Variables table to hold entry and extern variables to output to files
 Entry: all the .entry symbols, the address is the memory location of entry declaration
 Extern: All the .extern symbols, the address is the memory location of usef of extern
*/

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

void addVariable(struct Variable *current_variable, struct Variable *new_variable)
{
    /* Assumming head is already exists */

    /* Checking if current symbol is NULL */
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

/* Write evey entry variable to .ent file */
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

/* Write evey extern variable to .ext file */
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

void printVariableTable(struct Variable *head)
{
    struct Variable *temp = head;
    printf("Variable Table:\n");
    while (temp != NULL)
    {
        printf("\"%s\": address: %d, \"%s\" -> ", temp->name, temp->address, temp->type);
        temp = temp->next;
        printf("\n");
    }
    printf("=================\n");
}

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