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

#include "test_memory.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

namespace libasm {
namespace test {

TestMemory::TestMemory()
    : DisMemory(0),
      _bytes(nullptr),
      _words(nullptr)
{
}

void TestMemory::setMemory(const uint8_t *data, host::uint_t size) {
    _bytes = data;
    _words = nullptr;
    _size = size;
    _index = 0;
}

void TestMemory::setMemory(const uint16_t*data, host::uint_t size) {
    _bytes = nullptr;
    _words = data;
    _size = size;
    _index = 0;
}

bool TestMemory::hasNext() const {
    return _index < _size;
}

void TestMemory::setAddress(uint32_t addr) {
    this->_address = addr;
}

char *TestMemory::dump(char *out) {
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

uint8_t TestMemory::nextByte() {
    if (_words) {
        const uint16_t word = _words[_index / 2];
        // Big-endian word. Most significant byte is in lowest address.
        return (_index++ % 2) == 0 ? (word >> 8) : word;
    }
    return _bytes[_index++];
}

} // namespace test
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
