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

#ifndef __BIN_READER_H__
#define __BIN_READER_H__

#include <cstdint>

#include "dis_memory.h"

namespace libasm {
namespace driver {

struct BinReader {
    struct ByteReader : DisMemory {
        ByteReader(uint32_t base, uint32_t size, const uint8_t *data)
            : DisMemory(base), _size(size), _data(data) {}
        uint32_t size() const { return _size; }
        bool hasNext() const override { return _size > 0; };

    protected:
        uint8_t nextByte() override {
            --_size;
            return *_data++;
        }

    private:
        uint32_t _size;
        const uint8_t *_data;
    };

    struct Block {
        virtual uint32_t base() const = 0;
        virtual uint32_t size() const = 0;
        virtual const uint8_t *data() const = 0;
        virtual const Block *next() const = 0;
        virtual ByteReader reader() const = 0;
    };

    virtual const Block *begin() const = 0;

    uint32_t blocks() const {
        uint32_t blocks = 0;
        for (auto b = begin(); b != nullptr; b = b->next())
            ++blocks;
        return blocks;
    }

    uint32_t size() const {
        uint32_t size = 0;
        for (auto b = begin(); b != nullptr; b = b->next())
            size += b->size();
        return size;
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
