#ifndef EXTERN_H
#define EXTERN_H

/* Structure to represent an Extern node */
struct Extern
{
    const char *name;
    struct Extern *next;
};

/*
Create a new Extern node

@param *extern_name The extern symbol name
@return New extern node.
*/
struct Extern *createExtern(const char *extern_name);

/*
Add an extern at the end of the node list 

@param current_extern The current last extern node.
@param new_extern The new extern node to add.
*/
void addExtern(struct Extern *current_extern, struct Extern *new_extern);

/*
Find an entry in the extern table by its name.
 
@param head Pointer to the head of the extern table.
@param name The name to search for.
@return Pointer to the found extern entry if exists, otherwise NULL.
*/
struct Extern *findExtern(struct Extern *head, const char *name);

/*
Print the extern table 

@param head The head of the extern table.
*/
void printExternTable(struct Extern *head);

/*
Free the memory allocated by the extern table 

@param head A pointer to the head of the extern table.
*/
void freeExternTable(struct Extern **head);

#endif /* EXTERN_H */