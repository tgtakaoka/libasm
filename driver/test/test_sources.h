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

#ifndef __TEST_SOURCES_H__
#define __TEST_SOURCES_H__

#include "asm_sources.h"
#include "test_reader.h"

#include <list>
#include <map>

namespace libasm {
namespace driver {
namespace test {

class TestSources : public driver::AsmSources {
public:
    Error open(const StrScanner &name) override {
        const auto *parent = _sources.empty() ? nullptr : _sources.back();
        const auto pos = parent ? parent->name().find_last_of('/') : std::string::npos;
        std::string key;
        if (pos == std::string::npos || *name == '/') {
            key = std::string(name.str(), name.size());
        } else {
            key = std::string(parent->name().substr(0, pos + 1));
            key.append(name.str(), name.size());
        }
        auto it = _files.find(key);
        if (it == _files.end())
            return NO_INCLUDE_FOUND;
        _sources.push_back(it->second);
        _sources.back()->rewind();
        return OK;
    }

    driver::TextReader *current() override { return _sources.empty() ? nullptr : _sources.back(); }

    Error closeCurrent() override {
        _sources.pop_back();
        return OK;
    }

    size_t size() const override { return _sources.size(); }

    TestSources &add(TestReader &reader) {
        _files.emplace(reader.name(), &reader);
        return *this;
    }

private:
    std::list<TestReader *> _sources;
    std::map<std::string, TestReader *> _files;
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
