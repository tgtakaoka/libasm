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

#ifndef __LIBASM_BIN_MEMORY_H__
#define __LIBASM_BIN_MEMORY_H__

#include <map>
#include <vector>

#include "bin_reader.h"
#include "config_base.h"

namespace libasm {
namespace driver {

struct BinMemory : BinReader {
    BinMemory();

    bool hasByte(uint32_t addr) const;
    void writeByte(uint32_t addr, uint8_t val);
    uint8_t readByte(uint32_t addr) const;

    bool operator==(const BinMemory &other) const;
    bool operator!=(const BinMemory &other) const { return !(*this == other); }
    void swap(BinMemory &other);
    void clear();

    uint32_t startAddress() const;
    uint32_t endAddress() const;

    const BinReader::Block *begin() const override;

    void setRange(uint32_t start, uint32_t end);

private:
    struct Block : BinReader::Block {
        Block(uint32_t addr) : _base(addr), _data(), _next(nullptr) {}

        uint32_t base() const override { return _base; }
        uint32_t size() const override { return _data.size(); }
        const uint8_t *data() const override { return _data.data(); }
        const BinReader::Block *next() const override {
            return reinterpret_cast<const BinReader::Block *>(_next);
        }
        BinReader::ByteReader reader() const override {
            return BinReader::ByteReader{base(), size(), data()};
        }

        bool operator==(const Block &other) const;
        bool operator!=(const Block &other) const { return !(*this == other); }
        bool inside(uint32_t addr) const { return addr >= _base && addr < _base + _data.size(); }
        bool atEnd(uint32_t addr) const { return addr == _base + _data.size(); }
        uint8_t read(uint32_t addr) const { return _data[addr - _base]; }
        void write(uint32_t addr, uint8_t val) { _data[addr - _base] = val; }
        void append(uint8_t val) { _data.push_back(val); }
        void aggregate(const Block &other);
        void connect(const Block *next) { _next = next; }
        void trimStart(uint32_t start);
        void trimEnd(uint32_t end);

    private:
        uint32_t _base;
        std::vector<uint8_t> _data;
        const Block *_next;
    };

    // Memory block map in ascending order of start address.
    using Blocks = std::map<uint32_t, Block>;
    Blocks _blocks;
    Blocks::iterator find(uint32_t addr);
    Blocks::const_iterator find(uint32_t addr) const;

    void createBlock(uint32_t addr, uint8_t val);
    void aggregate(Block &block);
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
