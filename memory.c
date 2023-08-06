#include <stdio.h>
#include <string.h>

int promoteMemory(int memory_count, char *line, int num_words, char *name)
{
    printf("Memory before add: %d\n", memory_count);
    if (strcmp(name, ".data") == 0)
    {
        memory_count += num_words - 1;
    }
    else if (strcmp(name, ".string") == 0)
    {
        int len = strlen(line);
        memory_count += len - 2; /* removing 2 quotes  */
    }
    printf("Memory after add: %d\n", memory_count);
    return memory_count;
}