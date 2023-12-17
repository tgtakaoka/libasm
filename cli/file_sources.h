/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __LIBASM_FILE_SOURCES_H__
#define __LIBASM_FILE_SOURCES_H__

#include <list>
#include <string>

#include "asm_sources.h"
#include "file_reader.h"

namespace libasm {
namespace cli {

struct FileSources final : driver::AsmSources {
    Error open(const StrScanner &name) override {
        if (size() >= max_includes)
            return TOO_MANY_INCLUDE;
        const auto *parent = _sources.empty() ? nullptr : &_sources.back();
        const auto pos = parent ? parent->name().find_last_of('/') : std::string::npos;
        if (pos == std::string::npos || *name == '/') {
            _sources.emplace_back(std::string(name.str(), name.size()));
        } else {
            std::string path(parent->name().substr(0, pos + 1));
            path.append(name.str(), name.size());
            _sources.push_back(path);
        }
        if (!_sources.back().open()) {
            _sources.pop_back();
            return NO_INCLUDE_FOUND;
        }
        return OK;
    }

    Error closeCurrent() override {
        auto &reader = _sources.back();
        reader.close();
        _sources.pop_back();
        return OK;
    }

private:
    static constexpr int max_includes = 4;
    std::list<FileReader> _sources;

    int size() const override { return _sources.size(); }
    driver::TextReader *last() override { return _sources.empty() ? nullptr : &_sources.back(); }
    driver::TextReader *secondToLast() override {
        if (size() < 2)
            return nullptr;
        auto it = _sources.rbegin();
        return &(*++it);
    }
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
