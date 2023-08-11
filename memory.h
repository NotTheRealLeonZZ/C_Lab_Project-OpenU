#ifndef MEMORY_H
#define MEMORY_H

/* Here I re-created string.h commands I needed */

int promoteMemoryDirectory(int memory_count, char *line, int num_words, char *name);

int promoteMemoryInstruction(int memory_count, int num_words, char words[][MAX_LINE_LENGTH]);

#endif /* MEMORY_H */
