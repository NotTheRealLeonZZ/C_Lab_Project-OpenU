#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "binary.h"
#include "my_string.h"
#include "globals.h"

/* Create a new Binary node */
struct Binary *createBinary(const char *binary_code, const char *type)
{
    struct Binary *new_binary_code = (struct Binary *)malloc(sizeof(struct Binary));
    if (new_binary_code == NULL)
    {
        fprintf(stdout, "Memory allocation failed. Exiting...\n");
        exit(1);
    }

    new_binary_code->code = my_strdup(binary_code);
    new_binary_code->type = my_strdup(type);
    new_binary_code->next = NULL;

    return new_binary_code;
}

/* Add a binary code at the end of the node list */
void addBinary(struct Binary *current_binary_code, struct Binary *new_binary_code)
{
    /* Assumming head is already exists */

    /* Checking if current binary code is NULL (memory error) */
    if (current_binary_code == NULL)
    {
        fprintf(stdout, "The given current node cannot be NULL.\n");
        return;
    }

    /* Find last binary code and change binary code's next to point to the new binary code */
    while (current_binary_code->next != NULL)
    {
        current_binary_code = current_binary_code->next;
    }

    current_binary_code->next = new_binary_code;
}

/* Free the memory allocated by the binary code table */
void freeBinaryTable(struct Binary **head)
{
    struct Binary *current = *head;
    struct Binary *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp); /* Free the memory allocated for the node */
    }

    *head = NULL; /* Set the head pointer to NULL to indicate an empty list */
}