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
 * Byte/Word addressable memory from array constant
 */
struct ArrayMemory {
    /** DisMemory interface of ArrayMemory */
    struct Iterator : DisMemory {
        bool hasNext() const override { return _index < _memory.size(); }

        uint16_t readUint16() {
            uint16_t word = readByte();
            if (_endian == ENDIAN_BIG) {
                word = (word << 8) | readByte();
            } else {
                word |= readByte() << 8;
            }
            return word;
        }

        /** rewind to the first byte */
        void rewind() {
            DisMemory::resetAddress(_memory.origin() * _memory.unit());
            _index = 0;
        }

    private:
        friend ArrayMemory;
        Iterator(const ArrayMemory &memory, Endian endian)
            : DisMemory(memory.origin() * memory.unit()),
              _memory(memory),
              _endian(endian),
              _index(0) {}

        uint8_t nextByte() override {
            return _memory.byteAt(_memory.origin() * _memory.unit() + _index++);
        }

        const ArrayMemory &_memory;
        const Endian _endian;
        size_t _index;
    };

    /** Construct byte addressable memory from uint8_t array */
    ArrayMemory(uint32_t origin, const uint8_t *bytes, size_t sizeof_bytes,
            Endian endian = ENDIAN_BIG, AddressUnit = ADDRESS_BYTE)
        : _origin(origin),
          _bytes(bytes),
          _words(nullptr),
          _size(sizeof_bytes),
          _endian(endian),
          _unit(ADDRESS_BYTE) {}

    /** Construct byte/word addressable memory from uint16_t array */
    ArrayMemory(uint32_t origin, const uint16_t *words, size_t sizeof_words, Endian endian,
            AddressUnit unit)
        : _origin(origin * unit),
          _bytes(nullptr),
          _words(words),
          _size(sizeof_words),
          _endian(endian),
          _unit(unit) {}

    /** byte address of the first byte/word */
    uint32_t origin() const { return _origin / _unit; }

    /** byte address of the last byte/word */
    uint32_t end() const { return (_origin + size()) / _unit - 1; }

    /** memory size in byte */
    size_t size() const { return _size; }

    bool word() const { return _words != nullptr; }

    /** endianess */
    Endian endian() const { return _endian; }

    /** memory size in byte */
    AddressUnit unit() const { return _unit; }

    /** returns DisMemory interface with specified endian */
    Iterator iterator(Endian endian = ENDIAN_BIG) const { return Iterator(*this, endian); }

    /** returns byte at address |addr|, otherwise zero */
    uint8_t byteAt(uint32_t addr) const {
        const auto offset = addr - _origin;
        if (offset >= _size)
            return 0;
        if (_bytes)
            return _bytes[offset];
        const uint16_t word = _words[offset / 2];
        if (_endian == ENDIAN_BIG) {
            return (offset % 2) == 0 ? (word >> 8) : word;
        } else {
            return (offset % 2) == 0 ? word : (word >> 8);
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
    const AddressUnit _unit;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
