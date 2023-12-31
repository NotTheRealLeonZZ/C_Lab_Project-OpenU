#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symbol.h"
#include "registers.h"
#include "instructions.h"
#include "directives.h"
#include "my_string.h"
#include "globals.h"

/* Create a new symbol structure. */
struct Symbol *createSymbol(const char *symbol_name, const int symbol_address, const char *symbol_type)
{

    struct Symbol *new_symbol = (struct Symbol *)malloc(sizeof(struct Symbol));
    if (new_symbol == NULL)
    {
        fprintf(stdout, "Memory allocation failed. Exiting...\n");
        exit(1);
    }

    new_symbol->name = my_strdup(symbol_name);
    new_symbol->address = symbol_address;
    new_symbol->type = my_strdup(symbol_type);
    new_symbol->next = NULL;

    return new_symbol;
}

/* Add a symbol to the end of the symbol table. */
void addSymbol(struct Symbol *current_symbol, struct Symbol *new_symbol)
{
    /* Assumming head is already exists */

    /* Checking if current symbol is NULL (memory error) */
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

/* Find a symbol in the symbol table by its name. */
struct Symbol *findSymbol(struct Symbol *head, const char *name)
{
    struct Symbol *head_copy;

    /* Checking if head of macro table is NULL */
    if (head == NULL)
    {
        fprintf(stdout, "Cannot find symbol, empty table.");
        return NULL;
    }

    head_copy = head;

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

/* Check if a given word can be a symbol. */
bool wordIsSymbol(char *word)
{
    size_t word_length;
    size_t i;

    if (word == NULL || word == '\0')
    {
        /* Check if word is empty, so its not a symbol */
        return false;
    }

    word_length = strlen(word);

    /* Check if max symbol size exceeded */
    if (word_length > MAX_SYMBOL_NAME_LENGTH)
    {
        return false;
    }

    /* Making sure first character is a letter a-zA-Z */
    if (!isalpha(word[0]))
    {
        return false;
    }

    /* Check if the last character is ':' */
    if (word[word_length - 1] != ':')
    {
        return false;
    }

    /* Check all other words, to be numbers or letters */
    for (i = 1; i < word_length - 1; i++)
    {
        if (!isalnum(word[i]))
        {
            return false;
        }
    }

    /* Clear ':' from end of word */
    word[word_length - 1] = '\0';

    /* Check if its a name of saved word in our assembly (instruction, directive)
    It cannot be register, because register name starts with '@' and I already
    checked for first valid letter a-zA-Z */
    if (isDirectiveName(word) || isInstructionName(word))
    {
        return false;
    }

    /* All tests pass, its a valid symbol */
    return true;
}

/* Remove a symbol from a line. */
void removeSymbolFromLine(char *line, char *symbol_name)
{
    char *substr_pos = strstr(line, symbol_name);

    if (substr_pos != NULL)
    {
        /* Check what is the index where symbol name ends */
        int index = substr_pos - line + strlen(symbol_name);
        strcpy(symbol_name, line + index);
    }
    else
    {
        strcpy(symbol_name, line);
    }
}

/* Print a warning message if a symbol is of .entry or .extern. */
void warnSymbolIfNecessary(char *word, int line_number)
{
    if (strcmp(word, ".extern") == 0 || strcmp(word, ".entry") == 0)
    {
        fprintf(stdout, "Warning! in line %d: \".entry\" and \".extern\" shouldn't have a symbol! \n", line_number);
    }
}

/* Update the address of a symbol to reflect its final address. */
void fixSymbolAddress(struct Symbol *symbol_table_head_copy, int ic)
{
    struct Symbol *temp = symbol_table_head_copy;
    while (temp != NULL)
    {
        if (strcmp(temp->type, "dir") == 0)
        {
            temp->address += ic;
        }

        temp = temp->next;
    }
}

/* Free the memory allocated by the symbol table. */
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