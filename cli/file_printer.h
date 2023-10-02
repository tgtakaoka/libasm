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

#ifndef __FILE_PRINTER_H__
#define __FILE_PRINTER_H__

#include "text_printer.h"

#include <cstdio>
#include <string>

namespace libasm {
namespace cli {

struct FilePrinter : libasm::driver::TextPrinter {
    FilePrinter();
    ~FilePrinter();
    const std::string &name() const { return _name; }
    bool open(const std::string &name);
    void close();
    void println(const char *text) override;
    void format(const char *fmt, ...) override;

    static FilePrinter STDERR;

private:
    std::string _name;
    FILE *_file;

    FilePrinter(const char *name, FILE *file) : _name(name), _file(file) {}
};

}  // namespace cli
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
