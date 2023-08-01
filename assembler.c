#include <stdio.h>
#include "pre_assembler.h"
#include "parser.h"
#include "first_pass.h"

#define MAX_FILE_NAME_LENGTH 100

int main(int argc, char *argv[])
{
    /* Validate command line 
    Get the file name from the input */
    if (argc != 2) /* CHANGE IT LATER TO FIT MY NEEDS - MAYBE CHECK IF ALL FILE NAMES HAS .AS FILE IN THIS FOLDER */
    {
        fprintf(stderr, "Usage: %s <filename_without_extension>\n", argv[0]);
        return 1;
    }

    /* Extract the filename from the command-line argument - DO IT IN A LOOP FOR ALL ARGV */
    char as_filename[MAX_FILE_NAME_LENGTH];
    snprintf(as_filename, sizeof(as_filename), "%s.as", argv[1]);

    char am_filename[MAX_FILE_NAME_LENGTH];
    snprintf(am_filename, sizeof(am_filename), "%s.am", argv[1]);

    printf("as file name: %s\tam file name: %s\n", as_filename, am_filename);

    /* Spread macros over .am file */
    preAssemble(as_filename, am_filename);

    /* First pass - Create tables */
    firstPass(am_filename);

    return 0;
}
