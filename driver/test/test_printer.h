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

#ifndef __TEST_PRINTER_H__
#define __TEST_PRINTER_H__

#include "text_printer.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace libasm {
namespace driver {
namespace test {

class TestPrinter : public TextPrinter {
public:
    TestPrinter() : _buffer() {}

    void println(const char *text) override {
        _buffer += text;
        _buffer += '\n';
        split();
    }

    void format(const char *fmt, ...) override {
        std::va_list args1;
        va_start(args1, fmt);
        const auto len = std::vsnprintf(nullptr, 0, fmt, args1);
        char out[len + 1];
        std::va_list args2;
        va_start(args2, fmt);
        std::vsnprintf(out, len + 1, fmt, args2);
        _buffer += out;
        split();
    }

    void clear() { _lines.clear(); }
    size_t size() const { return _lines.size(); }
    const char *line(int lineno) const { return _lines[lineno - 1].c_str(); }

private:
    std::string _buffer;
    std::vector<std::string> _lines;

    void split() {
        for (auto pos = _buffer.find('\n'); pos != std::string::npos; pos = _buffer.find('\n')) {
            _lines.emplace_back(_buffer.c_str(), pos);
            _buffer = _buffer.substr(pos + 1);
        }
    }
};

}  // namespace test
}  // namespace driver
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
