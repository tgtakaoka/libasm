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
    virtual Error open(const StrScanner &name) = 0;
    virtual TextReader *current() = 0;
    virtual Error closeCurrent() = 0;
    virtual size_t size() const = 0;

    StrScanner *readLine() {
        TextReader *reader;
        while ((reader = current()) != nullptr) {
            auto *line = reader->readLine();
            if (line)
                return line;
            closeCurrent();
        }
        return nullptr;
    }
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
