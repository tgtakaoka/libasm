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

#include "bin_memory.h"

#include <cstdlib>
#include <cstring>

namespace libasm {
namespace driver {

BinMemory::BinMemory() {}

BinMemory::Blocks::iterator BinMemory::find(uint32_t addr) {
    if (_blocks.empty())
        return _blocks.end();
    auto it = _blocks.lower_bound(addr);
    if (it == _blocks.begin())
        return addr < it->second.base() ? _blocks.end() : it;
    if (it == _blocks.end() || addr < it->second.base())
        --it;
    return it;
}

BinMemory::Blocks::const_iterator BinMemory::find(uint32_t addr) const {
    if (_blocks.empty())
        return _blocks.cend();
    auto it = _blocks.lower_bound(addr);
    if (it == _blocks.cbegin())
        return addr < it->second.base() ? _blocks.cend() : it;
    if (it == _blocks.cend() || addr < it->second.base())
        --it;
    return it;
}

bool BinMemory::hasByte(uint32_t addr) const {
    auto it = find(addr);
    return it != _blocks.cend() && it->second.inside(addr);
}

void BinMemory::writeByte(uint32_t addr, uint8_t val) {
    auto it = find(addr);
    if (it == _blocks.end()) {
        createBlock(addr, val);
        return;
    }
    auto &block = it->second;
    if (block.inside(addr)) {
        block.write(addr, val);
    } else if (block.atEnd(addr)) {
        block.append(val);
        aggregate(block);
    } else {
        createBlock(addr, val);
    }
}

uint8_t BinMemory::readByte(uint32_t addr) const {
    auto it = find(addr);
    if (it != _blocks.cend()) {
        const auto &block = it->second;
        if (block.inside(addr))
            return block.read(addr);
    }
    return 0;
}

bool BinMemory::operator==(const BinMemory &other) const {
    return _blocks == other._blocks;
}

void BinMemory::swap(BinMemory &other) {
    _blocks.swap(other._blocks);
}

const BinReader::Block *BinMemory::begin() const {
    return _blocks.empty() ? nullptr : &_blocks.begin()->second;
}

void BinMemory::clear() {
    _blocks.clear();
}

uint32_t BinMemory::startAddress() const {
    return _blocks.empty() ? 0 : _blocks.begin()->second.base();
}

uint32_t BinMemory::endAddress() const {
    if (_blocks.empty())
        return 0;
    const auto it = _blocks.rbegin();
    return it->second.base() + it->second.size() - 1;
}

void BinMemory::setRange(uint32_t start, uint32_t end) {
    if (end < start)
        return;
    Block *b = nullptr;
    for (auto it = _blocks.begin(); it != _blocks.end();) {
        auto &block = it->second;
        const auto s = block.base();
        const auto e = block.base() + block.size() - 1;
        if (e < start || s > end) {
            // out of range
            it = _blocks.erase(it);
            continue;
        }
        if (s < start && e >= start)
            block.trimStart(start);
        if (s < end && e >= end)
            block.trimEnd(end);
        b = &block;
        ++it;
    }
    if (b)
        b->connect(nullptr);
}

void BinMemory::createBlock(uint32_t addr, uint8_t val) {
    auto it = _blocks.emplace(addr, addr).first;
    auto &block = it->second;
    auto next = it;
    if (++next != _blocks.end())
        block.connect(&next->second);
    auto prev = it;
    if (prev-- != _blocks.begin())
        prev->second.connect(&block);
    block.append(val);
    aggregate(block);
}

void BinMemory::aggregate(Block &block) {
    auto it = _blocks.find(block.base());
    if (it == _blocks.end())
        return;
    auto next = it;
    if (++next != _blocks.end() && it->second.atEnd(next->second.base())) {
        it->second.aggregate(next->second);
        _blocks.erase(next);
    }
    auto prev = it;
    if (prev-- != _blocks.begin() && prev->second.atEnd(it->second.base())) {
        prev->second.aggregate(it->second);
        _blocks.erase(it);
    }
}

bool BinMemory::Block::operator==(const Block &other) const {
    return _base == other._base && _data == other._data;
}

void BinMemory::Block::aggregate(const Block &other) {
    _data.reserve(_data.size() + other._data.size());
    _data.insert(_data.end(), other._data.begin(), other._data.end());
    _next = other._next;
}

void BinMemory::Block::trimStart(uint32_t start) {
    auto delta = start - _base;
    _data.erase(_data.begin(), _data.begin() + delta);
    _base = start;
}

void BinMemory::Block::trimEnd(uint32_t end) {
    auto delta = _base + _data.size() - 1 - end;
    _data.erase(_data.end() - delta, _data.end());
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
