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
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>

#include "bin_formatter.h"
#include "dis_memory.h"

template<typename Addr>
class CliMemory : public DisMemory<Addr> {
public:
    CliMemory() : DisMemory<Addr>(0) {
        invalidateWriteCache();
        invalidateReadCache();
    }

    void setAddress(Addr addr) {
        this->_address = addr;
    }

    bool hasNext() const override {
        Addr addr = this->_address;
        if (insideOf(_read_cache, addr)) return true;
        for (auto segment = _segments.cbegin();
             segment != _segments.cend();
             segment++) {
            if (insideOf(segment, addr)) {
                _read_cache = segment;
                return true;
            }
        }
        invalidateReadCache();
        return false;
    }

protected:
    uint8_t nextByte() override {
        uint8_t val = 0;
        readByte(this->_address, val);
        return val;
    }

public:
    void writeBytes(Addr addr, const uint8_t *p, size_t size) {
        for (const uint8_t *end = p + size; p < end; p++)
            writeByte(addr++, *p);
    }

    bool readBytes(Addr addr, uint8_t *p, size_t size) const {
        for (uint8_t *end = p + size; p < end; p++) {
            if (!readByte(addr, p))
                return false;
        }
        return true;
    }

    void writeByte(Addr addr, uint8_t val) {
        // Shortcut for most possible case, appending byte to cached segment.
        if (atEndOf(_write_cache, addr)) {
            appendTo(_write_cache, addr, val);
            return;
        }
        if (insideOf(_write_cache, addr)) {
            replaceAt(_write_cache, addr, val);
            return;
        }

        invalidateWriteCache();
        for (auto segment = _segments.begin();
             segment != _segments.end();
             segment++) {
            if (atEndOf(segment, addr)) {
                appendTo(segment, addr, val);
                return;
            }
            if (insideOf(segment, addr)) {
                replaceAt(segment, addr, val);
                return;
            }
        }

        // No appropriate segment found, create a segment.
        createSegment(addr, val);
    }

    bool readByte(Addr addr, uint8_t &val) const {
        if (insideOf(_read_cache, addr)) {
            val = readFrom(_read_cache, addr);
            return true;
        }
        for (auto segment = _segments.cbegin();
             segment != _segments.cend();
             segment++) {
            if (insideOf(segment, addr)) {
                val = readFrom(segment, addr);
                _read_cache = segment;
                return true;
            }
        }
        invalidateReadCache();
        return false;
    }

    bool equals(const CliMemory<Addr> &other) const {
        auto a = _segments.cbegin();
        auto b = other._segments.cbegin();
        while (a != _segments.cend() && b != other._segments.cend()) {
            if (a->first != b->first)
                return false;
            if (a->second.size() != b->second.size())
                return false;
            for (size_t i = 0; i < a->second.size(); i++) {
                if (a->second[i] != b->second[i])
                    return false;
            }
            a++;
            b++;
        }
        return a == _segments.cend() && b == other._segments.cend();
    }

    void swap(CliMemory& other) {
        _segments.swap(other._segments);
        invalidateWriteCache();
        other.invalidateWriteCache();
    }

    // Dumper should accept (Addr, const uint8_t *, size_t)
    template<typename Dumper>
    void dump(Dumper dumper) const {
        for (auto segment = _segments.cbegin();
             segment != _segments.cend(); segment++) {
            const auto &mem = segment->second;
            dumper(segment->first, mem.data(), mem.size());
        }
    }

private:
    // Memory segment list in ascending order of start address.
    typedef std::map<Addr, std::vector<uint8_t>> Segments;
    typedef typename Segments::iterator Segment;
    typedef typename Segments::const_iterator ConstSegment;
    Segments _segments;
    Segment _write_cache;
    mutable ConstSegment _read_cache;

    void invalidateWriteCache() {
        _write_cache = _segments.end();
    }

    void invalidateReadCache() const {
        _read_cache = _segments.cend();
    }

    bool insideOf(ConstSegment &segment, Addr addr) const {
        return segment != _segments.cend()
            && addr >= segment->first
            && addr < segment->first + segment->second.size();
    }

    bool insideOf(Segment segment, Addr addr) const {
        return segment != _segments.end()
            && addr >= segment->first
            && addr < segment->first + segment->second.size();
    }

    bool atEndOf(Segment segment, Addr addr) const {
        return segment != _segments.end()
            && addr == segment->first + segment->second.size();
    }

    uint8_t readFrom(ConstSegment &segment, Addr addr) const {
        return segment->second.at(addr - segment->first);
    }

    void appendTo(Segment &segment, Addr addr, uint8_t val) {
        segment->second.push_back(val);
        _write_cache = segment;
        aggregate(segment);
    }

    void replaceAt(Segment &segment, Addr addr, uint8_t val) {
        segment->second.at(addr - segment->first) = val;
        _write_cache = segment;
        aggregate(segment);
    }

    void createSegment(Addr addr, uint8_t val) {
        _write_cache = _segments.insert(
            std::make_pair(addr, std::vector<uint8_t>())).first;
        _write_cache->second.push_back(val);
        aggregate(_write_cache);
    }

    void aggregate(Segment hint) {
        Segment prev = hint;
        // Check following segment whether it is adjacent to.
        for (Segment next = ++hint;
             next != _segments.end() && atEndOf(prev, next->first);
             next = _segments.erase(next)) {
            // Append next segment.
            prev->second.reserve(prev->second.size() + next->second.size());
            prev->second.insert(
                prev->second.end(),
                std::make_move_iterator(next->second.begin()),
                std::make_move_iterator(next->second.end()));
            invalidateWriteCache();
        }
    }
};

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
