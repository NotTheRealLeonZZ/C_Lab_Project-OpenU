#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_string.h"

/* Duplicate a given string using memory allocation. */
char *my_strdup(const char *str)
{
    size_t len = strlen(str) + 1; /* +1 for the null-terminator */
    char *new_str = (char *)malloc(len);
    if (new_str == NULL)
    {
        return NULL; /* Memory allocation failed */
    }
    strcpy(new_str, str);
    return new_str;
}
