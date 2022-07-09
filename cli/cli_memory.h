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

#ifndef __CLI_MEMORY_H__
#define __CLI_MEMORY_H__

#include <stdint.h>

#include <map>
#include <vector>

#include "bin_formatter.h"
#include "dis_memory.h"

namespace libasm {
namespace cli {

class CliMemory : public DisMemory {
public:
    CliMemory();

    void setAddress(uint32_t addr);
    bool hasNext() const override;

protected:
    uint8_t nextByte() override;

public:
    void writeBytes(uint32_t addr, const uint8_t *p, size_t size);
    bool readBytes(uint32_t addr, uint8_t *p, size_t size) const;
    void writeByte(uint32_t addr, uint8_t val);
    bool readByte(uint32_t addr, uint8_t &val) const;
    bool equals(const CliMemory &other) const;
    void swap(CliMemory &other);

    uint32_t startAddress() const;
    uint32_t endAddress() const;

    // Iterator
    // it->first: uint32_t start_address;
    // it->second: vector<uint8_t> memory_block;
    auto begin() const { return _segments.begin(); }
    auto end() const { return _segments.end(); }

private:
    // Memory segment list in ascending order of start address.
    typedef std::map<uint32_t, std::vector<uint8_t>> Segments;
    typedef typename Segments::iterator Segment;
    typedef typename Segments::const_iterator ConstSegment;
    Segments _segments;
    Segment _write_cache;
    mutable ConstSegment _read_cache;

    void invalidateWriteCache();
    void invalidateReadCache() const;
    bool insideOf(ConstSegment &segment, uint32_t addr) const;
    bool insideOf(Segment segment, uint32_t addr) const;
    bool atEndOf(Segment segment, uint32_t addr) const;
    uint8_t readFrom(ConstSegment &segment, uint32_t addr) const;
    void appendTo(Segment &segment, uint32_t addr, uint8_t val);
    void replaceAt(Segment &segment, uint32_t addr, uint8_t val);
    void createSegment(uint32_t addr, uint8_t val);
    void aggregate(Segment hint);
};

}  // namespace cli
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
