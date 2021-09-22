/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "array_memory.h"

#include <inttypes.h>
#include <stdint.h>

namespace libasm {

ArrayMemory::ArrayMemory(uint32_t origin, const uint8_t *bytes, size_t size, Endian endian)
    : DisMemory(origin),
      _origin(origin),
      _bytes(bytes),
      _words(nullptr),
      _size(size),
      _endian(endian),
      _index(0) {}

ArrayMemory::ArrayMemory(uint32_t origin, const uint16_t *words, size_t size, Endian endian)
    : DisMemory(origin),
      _origin(origin),
      _bytes(nullptr),
      _words(words),
      _size(size),
      _endian(endian),
      _index(0) {}

bool ArrayMemory::hasNext() const {
    return _index < _size;
}

size_t ArrayMemory::size() const {
    return _size;
}

uint32_t ArrayMemory::origin() const {
    return _origin;
}

void ArrayMemory::rewind() {
    _index = 0;
}

uint8_t ArrayMemory::nextByte() {
    uint8_t byte;
    if (_bytes) {
        byte = _bytes[_index];
    } else {
        const uint16_t word = _words[_index / 2];
        const uint8_t pos = _index % 2;
        if (_endian == ENDIAN_BIG) {
            byte = (pos == 0) ? word >> 8 : word;
        } else {
            byte = (pos == 0) ? word : word >> 8;
        }
    }
    _index++;
    return byte;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
