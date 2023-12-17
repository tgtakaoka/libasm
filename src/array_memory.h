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

#ifndef __LIBASM_ARRAY_MEMORY_H__
#define __LIBASM_ARRAY_MEMORY_H__

#include "config_base.h"
#include "dis_memory.h"

#include <stddef.h>
#include <stdint.h>

namespace libasm {

/**
 * Byte addressable memory from array constant
 */
struct ArrayMemory {
    /** DisMemory interface of ArrayMemory */
    struct Iterator : DisMemory {
        bool hasNext() const override { return _index < _memory.size(); }

        /** rewind to the first byte */
        void rewind() {
            DisMemory::resetAddress(_memory.origin());
            _index = 0;
        }

    private:
        friend ArrayMemory;
        Iterator(const ArrayMemory &memory)
            : DisMemory(memory.origin()), _memory(memory), _index(0) {}
        uint8_t nextByte() override { return _memory.byteAt(_memory.origin() + _index++); }

        const ArrayMemory &_memory;
        size_t _index;
    };

    /** Construct byte addressable memory from uint8_t array */
    ArrayMemory(
            uint32_t origin, const uint8_t *bytes, size_t sizeof_bytes, Endian endian = ENDIAN_BIG)
        : _origin(origin), _bytes(bytes), _words(nullptr), _size(sizeof_bytes), _endian(endian) {}

    /** Construct byte addressable memory from uint16_t array with specified endian */
    ArrayMemory(uint32_t origin, const uint16_t *words, size_t sizeof_words, Endian endian)
        : _origin(origin), _bytes(nullptr), _words(words), _size(sizeof_words), _endian(endian) {}

    /** byte address of the first byte */
    uint32_t origin() const { return _origin; }

    /** byte address of the last byte */
    uint32_t end() const { return _origin + size() - 1; }

    /** memory size in byte */
    size_t size() const { return _size; }

    /** returns DisMemory interface */
    Iterator iterator() const { return Iterator(*this); }

    /** returns byte at address |addr|, otherwise zero */
    uint8_t byteAt(uint32_t addr) const {
        const auto offset = addr - _origin;
        if (offset >= _size)
            return 0;
        if (_bytes)
            return _bytes[offset];
        const uint16_t word = _words[offset / 2];
        const uint8_t hi = word >> 8;
        const uint8_t lo = word;
        if (offset % 2 == 0) {
            return _endian == ENDIAN_BIG ? hi : lo;
        } else {
            return _endian == ENDIAN_BIG ? lo : hi;
        }
    }

private:
    const uint32_t _origin;
    /** one of |_bytes| or |_words| is non-null, and others is null */
    const uint8_t *const _bytes;
    const uint16_t *const _words;
    /** size in byte */
    const size_t _size;
    const Endian _endian;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
