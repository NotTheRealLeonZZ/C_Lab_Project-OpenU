/* This is for 1st pass*/

void validateSyntax(char* instruction, char** operands) {
    if (strcmp(instruction, "ADD") == 0) {
        /* ADD instruction should have three operands: dest, src1, src2 */
        if (operands[0] == NULL || operands[1] == NULL || operands[2] == NULL) {
            fprintf(stderr, "Syntax Error: ADD instruction requires three operands.\n");
        }
        /* Validate the format of each operand, e.g., check if they are valid registers or immediate values */
        /* ... */
    } else if (strcmp(instruction, "SUB") == 0) {
        /* SUB instruction should have three operands: dest, src1, src2 */
        if (operands[0] == NULL || operands[1] == NULL || operands[2] == NULL) {
            fprintf(stderr, "Syntax Error: SUB instruction requires three operands.\n");
        }
        /* Validate the format of each operand, e.g., check if they are valid registers or immediate values */
        /* ... */
    } else {
        fprintf(stderr, "Syntax Error: Invalid instruction.\n");
    }
}

