#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "binary.h"
#include "my_string.h"
#include "globals.h"

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

    /* Checking if current binary code is NULL */
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

/* Function to find a binary code by name.
@param head - the first pointer to the binary code table
@param code - the binary code's code you looking for
@return pointer for the found binary code or NULL */
struct Binary *findBinary(struct Binary *head, const char *code)
{
    struct Binary *head_copy = head;

    /* Checking if head of binary code table is NULL */
    if (head == NULL)
    {
        fprintf(stdout, "Cannot find binary code, empty table.");
        return NULL;
    }

    while (head_copy != NULL)
    {
        if (strcmp(head_copy->code, code) == 0)
        {
            return head_copy;
        }
        head_copy = head_copy->next;
    }
    return NULL;
}

/* Print the binary code table */
void printBinaryTable(struct Binary *head)
{
    struct Binary *temp = head;
    printf("Binary Table:\n");
    while (temp != NULL)
    {
        printf("\"%s\" , %s", temp->code, temp->type);
        temp = temp->next;
        printf("\n");
    }
    printf("=================\n");
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