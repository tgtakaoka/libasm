/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
