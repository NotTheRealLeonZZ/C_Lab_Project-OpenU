#ifndef VARIABLES_H
#define VARIABLES_H

/* Structure to represent an entry or extern variable,
Will use this node list to print it to .ext and .ent files */
struct Variable
{
    const char *name;
    int address;
    const char *type; /* external (ext) or entry (ent) */
    struct Variable *next;
};

/* Create a new Variable structure */
struct Variable *createVariable(const char *variable_name, const int variable_address, const char *variable_type);

/* Add a variable at the end of the node list */
void addVariable(struct Variable *current_variable, struct Variable *new_variable);

/* Print the variable table */
void printVariableTable(struct Variable *head);

/* Free the memory allocated by the symbol table */
void freeVariableTable(struct Variable **head);

#endif /* VARIABLES_H */