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

#include "file_reader.h"

#include <cstdlib>
#include <cstring>

namespace libasm {
namespace cli {

static int getLine(char *&lineptr, size_t &n, FILE *input) {
    char *p = lineptr;
    auto max = n - 1;
    while (fgets(p, max, input)) {
        const auto len = strlen(lineptr);
        if (len > 0 && lineptr[len - 1] == '\n') {
            lineptr[len - 1] = 0;
            return len;
        }
        max = 128;
        n += max;
        lineptr = static_cast<char *>(realloc(lineptr, n));
        p = &lineptr[len];
    }
    return -1;
}

FileReader::FileReader(const std::string &name)
    : _name(name), _file(nullptr), _lineno(0), _line_len(80) {
    _line = static_cast<char *>(malloc(_line_len));
}

FileReader::~FileReader() {
    close();
    free(_line);
}

StrScanner *FileReader::readLine() {
    if (_file == nullptr || getLine(_line, _line_len, _file) < 0) {
        close();
        return nullptr;
    }
    ++_lineno;
    _line_scan = _line;
    return &_line_scan;
}

bool FileReader::open() {
    _file = fopen(_name.c_str(), "r");
    return _file != nullptr;
}

void FileReader::close() {
    if (_file)
        fclose(_file);
    _file = nullptr;
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
