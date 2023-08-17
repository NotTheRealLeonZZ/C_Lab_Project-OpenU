#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/*
Main Assmebler file.
Here I receive arguments from the user in terminal with the name of the .as files (without the .as extention)
For each file:
1) Pre-assemble and spread macros
2) First pass - Create symbol table, calculate memory + IC + DC, create extern table, check for syntax error and max file memory size.
3) Second pass - Validate all operands, validate .exntry and .extern, re-calculate memory + IC + DC, encode each line to binary
and build binary table, create variables table to output to files later.
4) If both passes pass, I check if need to create .ent and .ext files and create them from variable table
5) Create .ob file that encode the binary table to base64, instruction first and then directives.
*/

/* The actual assembler, running on each agrument the user entered */
void runAssembler(char *argv[], int index);

#endif /* ASSEMBLER_H */