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

#include "cli_memory.h"

#include <stdlib.h>
#include <string.h>

namespace libasm {
namespace cli {

CliMemory::CliMemory() : DisMemory(0) {
    invalidateWriteCache();
    invalidateReadCache();
}

void CliMemory::setAddress(uint32_t addr) {
    resetAddress(addr);
}

bool CliMemory::hasNext() const {
    const auto addr = address();
    if (insideOf(_read_cache, addr))
        return true;
    for (auto segment = _segments.cbegin(); segment != _segments.cend(); segment++) {
        if (insideOf(segment, addr)) {
            _read_cache = segment;
            return true;
        }
    }
    invalidateReadCache();
    return false;
}

uint8_t CliMemory::nextByte() {
    uint8_t val = 0;
    readByte(address(), val);
    return val;
}

void CliMemory::writeBytes(uint32_t addr, const uint8_t *p, size_t size) {
    for (const auto end = p + size; p < end; p++)
        writeByte(addr++, *p);
}

bool CliMemory::readBytes(uint32_t addr, uint8_t *p, size_t size) const {
    for (const auto end = p + size; p < end; p++) {
        if (!readByte(addr, *p))
            return false;
    }
    return true;
}

void CliMemory::writeByte(uint32_t addr, uint8_t val) {
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
    for (auto segment = _segments.begin(); segment != _segments.end(); segment++) {
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

bool CliMemory::readByte(uint32_t addr, uint8_t &val) const {
    if (insideOf(_read_cache, addr)) {
        val = readFrom(_read_cache, addr);
        return true;
    }
    for (auto segment = _segments.cbegin(); segment != _segments.cend(); segment++) {
        if (insideOf(segment, addr)) {
            val = readFrom(segment, addr);
            _read_cache = segment;
            return true;
        }
    }
    invalidateReadCache();
    return false;
}

bool CliMemory::equals(const CliMemory &other) const {
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

void CliMemory::swap(CliMemory &other) {
    _segments.swap(other._segments);
    invalidateWriteCache();
    other.invalidateWriteCache();
}

uint32_t CliMemory::startAddress() const {
    if (_segments.empty())
        return 0;
    const auto it = _segments.cbegin();
    return it->first;
}

uint32_t CliMemory::endAddress() const {
    if (_segments.empty())
        return 0;
    const auto it = _segments.crbegin();
    return it->first + it->second.size() - 1;
}

void CliMemory::invalidateWriteCache() {
    _write_cache = _segments.end();
}

void CliMemory::invalidateReadCache() const {
    _read_cache = _segments.cend();
}

bool CliMemory::insideOf(ConstSegment &segment, uint32_t addr) const {
    return segment != _segments.cend() && addr >= segment->first &&
           addr < segment->first + segment->second.size();
}

bool CliMemory::insideOf(Segment segment, uint32_t addr) const {
    return segment != _segments.end() && addr >= segment->first &&
           addr < segment->first + segment->second.size();
}

bool CliMemory::atEndOf(Segment segment, uint32_t addr) const {
    return segment != _segments.end() && addr == segment->first + segment->second.size();
}

uint8_t CliMemory::readFrom(ConstSegment &segment, uint32_t addr) const {
    return segment->second.at(addr - segment->first);
}

void CliMemory::appendTo(Segment &segment, uint32_t addr, uint8_t val) {
    segment->second.push_back(val);
    _write_cache = segment;
    aggregate(segment);
}

void CliMemory::replaceAt(Segment &segment, uint32_t addr, uint8_t val) {
    segment->second.at(addr - segment->first) = val;
    _write_cache = segment;
    aggregate(segment);
}

void CliMemory::createSegment(uint32_t addr, uint8_t val) {
    _write_cache = _segments.insert(std::make_pair(addr, std::vector<uint8_t>())).first;
    _write_cache->second.push_back(val);
    aggregate(_write_cache);
}

void CliMemory::aggregate(Segment hint) {
    Segment prev = hint;
    // Check following segment whether it is adjacent to.
    for (auto next = ++hint; next != _segments.end() && atEndOf(prev, next->first);
            next = _segments.erase(next)) {
        // Append next segment.
        prev->second.reserve(prev->second.size() + next->second.size());
        prev->second.insert(prev->second.end(), std::make_move_iterator(next->second.begin()),
                std::make_move_iterator(next->second.end()));
        invalidateWriteCache();
    }
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
