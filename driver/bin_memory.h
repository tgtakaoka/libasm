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

#ifndef __BIN_MEMORY_H__
#define __BIN_MEMORY_H__

#include "dis_memory.h"

#include <set>
#include <vector>

namespace libasm {
namespace driver {

class BinMemory {
public:
    BinMemory();

    bool hasByte(uint32_t addr) const;
    void writeByte(uint32_t addr, uint8_t val);
    uint8_t readByte(uint32_t addr) const;

    bool equals(const BinMemory &other) const;
    void swap(BinMemory &other);

    uint32_t startAddress() const;
    uint32_t endAddress() const;

    // byte iterator
    class ByteIterator : public DisMemory {
    public:
        bool hasNext() const override { return _memory.hasByte(address()); }
        void setAddress(uint32_t addr) { DisMemory::resetAddress(addr); }

    private:
        friend class BinMemory;
        ByteIterator(const BinMemory &memory, uint32_t addr) : DisMemory(addr), _memory(memory) {}
        uint8_t nextByte() override { return _memory.readByte(address()); }

        const BinMemory &_memory;
    };

    auto reader(uint32_t addr) const { return ByteIterator(*this, addr); }

    // block iterator
    // it->base: uint32_t start_address;
    // it->data: vector<uint8_t> memory_block;
    auto begin() const { return _blocks.begin(); }
    auto end() const { return _blocks.end(); }

private:
    struct Block {
        const uint32_t base;
        mutable std::vector<uint8_t> data;

        Block(uint32_t addr) : base(addr), data() {}
        bool operator<(const Block &o) const { return base < o.base; }
    };

    // Memory block set in ascending order of start address.
    std::set<Block> _blocks;
    typedef std::set<Block>::iterator Cache;
    Cache _writeCache;
    mutable Cache _readCache;

    void invalidateWriteCache();
    void invalidateReadCache() const;
    bool insideOf(Cache &cache, uint32_t addr) const;
    bool atEndOf(Cache &cache, uint32_t addr) const;
    uint8_t readFrom(Cache &cache, uint32_t addr) const;
    void appendTo(Cache &cache, uint32_t addr, uint8_t val);
    void replaceAt(Cache &cache, uint32_t addr, uint8_t val);
    void createBlock(uint32_t addr, uint8_t val);
    void aggregate(Cache &hint);
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
