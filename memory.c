#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "instructions.h"
#include "registers.h"

int promoteMemoryDirective(int memory_count, char *line, int num_words, char *name)
{
    int len;
    if (strcmp(name, ".data") == 0)
    {
        memory_count += num_words - 2;
    }
    else if (strcmp(name, ".string") == 0)
    {
        removeTrailingSpaces(line);
        len = strlen(line);
        printf("string: %s\n", line);
        memory_count += len - 2; /* removing 2 quotes  */
    }

    return memory_count;
}

int promoteMemoryInstruction(int memory_count, int num_words, char words[][MAX_LINE_LENGTH])
{
    int num_operands = num_words - 1;
    char operand1[MAX_SYMBOL_NAME_LENGTH];
    char operand2[MAX_SYMBOL_NAME_LENGTH];

    if (num_operands == 2)
    {
        strcpy(operand1, words[1]);
        strcpy(operand2, words[2]);

        if (isRegisterName(operand1) && isRegisterName(operand2))
            memory_count += 1;
        else

            memory_count += 2;
    }
    else if (num_operands == 1)
    {
        memory_count += 1;
    }

    return memory_count;
}