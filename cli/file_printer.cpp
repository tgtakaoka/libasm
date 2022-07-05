/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "file_printer.h"

#include <stdarg.h>

namespace libasm {
namespace cli {

FilePrinter::FilePrinter() : _name(), _file(nullptr) {}

FilePrinter::~FilePrinter() {
    if (_file)
        fclose(_file);
}

bool FilePrinter::open(const std::string &name) {
    _file = fopen(name.c_str(), "w");
    if (_file)
        _name = name;
    return _file != nullptr;
}

void FilePrinter::close() {
    if (_file)
        fclose(_file);
}

void FilePrinter::println(const char *text) {
    if (_file == nullptr)
        return;
    fprintf(_file, "%s\n", text);
    fflush(_file);
}

void FilePrinter::format(const char *fmt, ...) {
    if (_file == nullptr)
        return;
    va_list args;
    va_start(args, fmt);
    vfprintf(_file, fmt, args);
    fflush(_file);
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
