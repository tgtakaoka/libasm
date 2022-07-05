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

#ifndef __FILE_READER_H__
#define __FILE_READER_H__

#include "text_reader.h"

#include <stdio.h>

namespace libasm {
namespace cli {

class FileReader : public TextReader {
public:
    FileReader(const std::string &name);
    ~FileReader();
    const std::string &name() const override { return _name; }
    int lineno() const override { return _lineno; }
    StrScanner *readLine() override;
    bool open();
    void close();

private:
    const std::string _name;
    FILE *_file;
    int _lineno;
    size_t _line_len;
    char *_line;
    StrScanner _line_scan;
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
