#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

/* 
Pre assembler function.
The main mission here is to replace all the macros in the original input file (.as)
and make the file ready for the "First Assembler Pass" by outputing a new file (.am) with all the macros spread.
Comment lines must start with ; as first char

@param input_file the .as file name
@param output_file the .am file name 
*/
void preAssemble(const char *input_file, char *output_file);

#endif /* PRE_ASSEMBLER_H */
