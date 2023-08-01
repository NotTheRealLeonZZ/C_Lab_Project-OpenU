#include <stdlib.h>
#include <string.h>
#include "my_string.h"

/* Here I re-created string.h commands I needed */

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
