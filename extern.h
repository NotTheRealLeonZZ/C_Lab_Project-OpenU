#ifndef EXTERN_H
#define EXTERN_H

#include <stdbool.h>

/* Structure to represent a symbol */
struct Extern
{
    const char *name;
    struct Extern *next;
};

/* Create a new Symbol structure */
struct Extern *createExtern(const char *extern_name);

/* Add a symbol at the end of the node list */
void addExtern(struct Extern *current_extern, struct Extern *new_extern);

/* Find extern in extern table */
struct Extern *findExtern(struct Extern *head, const char *name);

/* Print the extern table */
void printExternTable(struct Extern *head);

/* Free the memory allocated by the extern table */
void freeExternTable(struct Extern **head);

#endif /* EXTERN_H */