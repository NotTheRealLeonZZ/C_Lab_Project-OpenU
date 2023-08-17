#ifndef MEMORY_H
#define MEMORY_H

/* This file will promote the memory the correct way, accourding to the parameters and instruction/directives */

/*
Promote memory for a directive line and update memory count accordingly.

@param memory_count The current memory count.
@param line The directive line.
@param num_words The number of words in the line.
@param name The name of the directive.
@return The updated memory count after promoting memory for the directive.
*/

int promoteMemoryDirective(int memory_count, char *line, int num_words, char *name);

/*
Promote memory for an instruction line and update memory count accordingly.

@param memory_count The current memory count.
@param num_words The number of words in the line.
@param words The words in the instruction line.
@return The updated memory count after promoting memory for the instruction.
*/
int promoteMemoryInstruction(int memory_count, int num_words, char words[][MAX_LINE_LENGTH]);

#endif /* MEMORY_H */
