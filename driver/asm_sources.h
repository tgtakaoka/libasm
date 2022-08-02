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

#ifndef __ASM_SOURCES_H__
#define __ASM_SOURCES_H__

#include "text_reader.h"

#include "error_reporter.h"

namespace libasm {
namespace driver {

class AsmSources {
public:
    /** open input source */
    virtual Error open(const StrScanner &name) = 0;

    /** read one line from current source, null if no remaining */
    StrScanner *readLine() {
        TextReader *reader;
        while ((reader = last()) != nullptr) {
            auto *line = reader->readLine();
            if (line)
                return line;
            closeCurrent();
        }
        return nullptr;
    }

    /** returns a nest level of just returned line */
    int nest() {
        auto reader = last();
        if (reader == nullptr)
            return 0;
        // if a line number is zero, it means just after opened.
        return reader->lineno() ? size() : size() - 1;
    }

    /** returns input source of just returned line */
    TextReader *current() {
        auto reader = last();
        if (reader == nullptr)
            return nullptr;
        // if a line number is zero, it means just after opened.
        return (reader->lineno() || size() == 1) ? reader : secondToLast();
    }

    virtual Error closeCurrent() = 0;

protected:
    /** returns number of input sources currently opened */
    virtual int size() const = 0;

    /** return input source of the last opened, null if there is no input source */
    virtual TextReader *last() = 0;

    /** return input source of the second to last, null if it not exists */
    virtual TextReader *secondToLast() = 0;
};

}  // namespace driver
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
