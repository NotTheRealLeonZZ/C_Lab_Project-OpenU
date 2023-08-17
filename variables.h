#ifndef VARIABLES_H
#define VARIABLES_H

/*
 Variables table to hold entry and extern variables to output to files
 Entry: all the .entry symbols, the address is the memory location of entry declaration
 Extern: All the .extern symbols, the address is the memory location of usef of extern
*/

/* Structure to represent an entry or extern variable,
Will use this node list to print it to .ext and .ent files */
struct Variable
{
    const char *name;      /* Variable name */
    int address;           /* Variable address */
    const char *type;      /* external (ext) or entry (ent) */
    struct Variable *next; /* Next pointer to next variable */
};

/*
Create a new Variable structure.
 
@param variable_name The name of the variable.
@param variable_address The address of the variable.
@param variable_type The type of the variable.
@return Pointer to the newly created Variable structure.
*/
struct Variable *createVariable(const char *variable_name, const int variable_address, const char *variable_type);

/*
Add a variable at the end of the node list.

@param current_variable Pointer to the current last node in the variable list.
@param new_variable Pointer to the new variable to be added.
*/
void addVariable(struct Variable *current_variable, struct Variable *new_variable);

/*
Write the entry variables to a file.

@param ent_file Pointer to the entry file.
@param head Pointer to the head of the variable table.
*/
void writeEntVariablesToFile(FILE *ent_file, struct Variable *head);

/*
Write the extern variables to a file.

@param ext_file Pointer to the extern file.
@param head Pointer to the head of the variable table.
*/
void writeExtVariablesToFile(FILE *ext_file, struct Variable *head);

/*
Free the memory allocated by the variable table.

@param head Pointer to the pointer of the variable table.
*/
void freeVariableTable(struct Variable **head);

#endif /* VARIABLES_H */