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
      _size(0),
      _bytes(nullptr),
      _origin(0),
      _length(0),
      _index(0),
      _words(nullptr) {
    ensureBytes(16);
}

TestMemory::~TestMemory() {
    delete[] _bytes;
}

void TestMemory::ensureBytes(size_t size) {
    if (_size >= size)
        return;
    delete[] _bytes;
    _bytes = new uint8_t[size];
    _size = size;
}

void TestMemory::setMemory(uint32_t addr, const uint8_t *data, uint8_t size) {
    _words = nullptr;
    ensureBytes(size);
    memcpy(_bytes, data, size);
    _origin = addr;
    resetAddress(addr);
    _length = size;
    _index = 0;
}

void TestMemory::setMemory(uint32_t addr, const uint16_t *data, uint8_t size, Endian endian) {
    _words = data;
    const uint8_t hi = (endian == ENDIAN_BIG) ? 0 : 1;
    const uint8_t lo = (endian == ENDIAN_BIG) ? 1 : 0;
    ensureBytes(size);
    for (uint8_t i = 0; i < size; i += 2) {
        const uint16_t d = *data++;
        _bytes[i + hi] = static_cast<uint8_t>(d >> 8);
        _bytes[i + lo] = static_cast<uint8_t>(d);
    }
    _origin = addr;
    resetAddress(addr);
    _length = size;
    _index = 0;
}

bool TestMemory::hasNext() const {
    return _index < _length;
}

const uint8_t *TestMemory::bytes() const {
    return _bytes;
}

uint8_t TestMemory::length() const {
    return _length;
}

uint32_t TestMemory::origin() const {
    return _origin;
}

char *TestMemory::dump(char *out) const {
    if (_words) {
        for (uint8_t idx = 0; idx < _length; idx += 2) {
            sprintf(out, "%04" PRIX16 " ", _words[idx / 2]);
            out += 8;
        }
        return out;
    }
    for (uint8_t idx = 0; idx < _length; idx++) {
        sprintf(out, "%02" PRIX8 " ", _bytes[idx]);
        out += 3;
    }
    return out;
}

uint8_t TestMemory::nextByte() {
    return _bytes[_index++];
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
