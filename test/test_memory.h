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

#ifndef __TEST_MEMORY_H__
#define __TEST_MEMORY_H__

#include "dis_memory.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

namespace libasm {
namespace test {

class TestMemory : public DisMemory<target::uintptr_t> {
public:
    TestMemory()
        : DisMemory(0),
          _bytes(nullptr),
          _words(nullptr)
    {}

    template<typename T>
    void setMemory(const T *data, host::uint_t size);
    bool hasNext() const override { return _index < _size; }
    void setAddress(target::uintptr_t addr) { _address = addr; }
    char *dump(char *out) {
        if (_words) {
            for (host::uint_t idx = 0; idx < _size; idx += 2) {
                sprintf(out, "%04" PRIX16 " ", _words[idx / 2]);
                out += 8;
            }
            return out;
        }
        for (host::uint_t idx = 0; idx < _size; idx++) {
            sprintf(out, "%02" PRIX8 " ", _bytes[idx]);
            out += 3;
        }
        return out;
    }

protected:
    uint8_t nextByte() override {
        if (_words) {
            const uint16_t word = _words[_index / 2];
            // Big-endian word. Most significant byte is in lowest address.
            return (_index++ % 2) == 0 ? (word >> 8) : word;
        }
        return _bytes[_index++];
    }

private:
    const uint8_t *_bytes;
    const uint16_t *_words;
    host::uint_t _size;
    host::uint_t _index;
};

template<>
void TestMemory::setMemory<uint8_t>(const uint8_t *data, host::uint_t size) {
    _bytes = data;
    _words = nullptr;
    _size = size;
    _index = 0;
}

template<>
void TestMemory::setMemory<uint16_t>(const uint16_t *data, host::uint_t size) {
    _bytes = nullptr;
    _words = data;
    _size = size;
    _index = 0;
}

} // namespace test
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
