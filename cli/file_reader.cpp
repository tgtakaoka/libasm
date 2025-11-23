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

ssize_t FileReader::getLine() {
    if (_line == nullptr)
        return -1;
    auto len = 0;  // current size
    auto available = _line_size;
    while (fgets(_line + len, available, _file)) {
        len += strlen(_line + len);
        if (len > 0 && _line[len - 1] == '\n') {
            _line[--len] = 0;  // remove end of line '\n'
            return len;
        }
        _line_size += GROW_BY;
        auto new_line = static_cast<char *>(realloc(_line, _line_size));
        if (new_line == nullptr) {
            free(_line);
            _line = nullptr;
            return -1;
        }
        _line = new_line;
        available = _line_size - len;
    }
    return -1;
}

FileReader::FileReader(const std::string &name)
    : _name(name), _file(nullptr), _lineno(0), _line_size(GROW_BY) {
    _line = static_cast<char *>(malloc(_line_size));
}

FileReader::~FileReader() {
    close();
    free(_line);
}

StrScanner *FileReader::readLine() {
    if (_file == nullptr || getLine() < 0) {
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
