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

#include <stdio.h>

#ifndef __TEST_READER_H__
#define __TEST_READER_H__

#include "text_reader.h"

#include <string>
#include <vector>

namespace libasm {
namespace driver {
namespace test {

class TestReader : public TextReader {
public:
    TestReader(const char *name) { clear(name); }

    const std::string &name() const override { return _name; }
    int lineno() const override { return _read; }
    StrScanner *readLine() override {
        if (_read >= _lines.size())
            return nullptr;
        _scan = StrScanner(_lines[_read++].c_str());
        return &_scan;
    }

    void rewind() { _read = 0; }

    TestReader &clear(const char *name) {
        _name = name;
        _lines.clear();
        rewind();
        return *this;
    }

    TestReader &add(std::string line) {
        auto start = line.begin();
        for (auto pos = line.begin(); pos != line.end(); pos++) {
            if (*pos == '\n') {
                _lines.emplace_back(start, pos);
                start = pos + 1;
            }
        }
        if (start < line.end())
            _lines.emplace_back(start, line.end());
        return *this;
    }

private:
    std::string _name;
    size_t _read;
    StrScanner _scan;
    std::vector<std::string> _lines;
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
