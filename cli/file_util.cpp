#include "file_util.h"

#include <string.h>
#include <stdlib.h>

int getLine(char *&lineptr, size_t &n, FILE *input) {
    char *p = lineptr;
    int max = n - 1;
    while (fgets(p, max, input)) {
        int len = strlen(lineptr);
        if (len > 0 && lineptr[len - 1] == '\n') {
            lineptr[len - 1] = 0;
            return len;
        }
        n += 128;
        lineptr = static_cast<char *>(realloc(lineptr, n));
        p = &lineptr[len - 1];
        max = 128;
    }
    return -1;
}

