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

BinMemory::BinMemory() {
    invalidateWriteCache();
    invalidateReadCache();
}

void BinMemory::invalidateWriteCache() {
    _writeCache = _blocks.end();
}

void BinMemory::invalidateReadCache() const {
    _readCache = _blocks.end();
}

bool BinMemory::hasByte(uint32_t addr) const {
    if (insideOf(_readCache, addr))
        return true;
    invalidateReadCache();

    for (auto cache = _blocks.begin(); cache != _blocks.end(); cache++) {
        if (insideOf(cache, addr)) {
            _readCache = cache;
            return true;
        }
    }
    return false;
}

void BinMemory::writeByte(uint32_t addr, uint8_t val) {
    // Shortcut for most possible case, appending byte to cached cache.
    if (atEndOf(_writeCache, addr)) {
        appendTo(_writeCache, addr, val);
        return;
    }
    if (insideOf(_writeCache, addr)) {
        replaceAt(_writeCache, addr, val);
        return;
    }
    invalidateWriteCache();

    for (auto cache = _blocks.begin(); cache != _blocks.end(); cache++) {
        if (atEndOf(cache, addr)) {
            appendTo(cache, addr, val);
            return;
        }
        if (insideOf(cache, addr)) {
            replaceAt(cache, addr, val);
            return;
        }
    }

    // No appropriate cache found, create a block.
    createBlock(addr, val);
}

uint8_t BinMemory::readByte(uint32_t addr) const {
    if (insideOf(_readCache, addr))
        return readFrom(_readCache, addr);
    invalidateReadCache();

    for (auto cache = _blocks.begin(); cache != _blocks.end(); cache++) {
        if (insideOf(cache, addr)) {
            _readCache = cache;
            return readFrom(cache, addr);
        }
    }
    return 0;
}

bool BinMemory::operator==(const BinMemory &other) const {
    return _blocks == other._blocks;
}

void BinMemory::swap(BinMemory &other) {
    _blocks.swap(other._blocks);
    invalidateReadCache();
    invalidateWriteCache();
    other.invalidateReadCache();
    other.invalidateWriteCache();
}

void BinMemory::clear() {
    _blocks.clear();
    invalidateReadCache();
    invalidateWriteCache();
}

uint32_t BinMemory::startAddress() const {
    return _blocks.empty() ? 0 : _blocks.begin()->base;
}

uint32_t BinMemory::endAddress() const {
    if (_blocks.empty())
        return 0;
    const auto cache = _blocks.rbegin();
    return cache->base + cache->data.size() - 1;
}

bool BinMemory::insideOf(Cache &cache, uint32_t addr) const {
    return cache != _blocks.end() && addr >= cache->base && addr < cache->base + cache->data.size();
}

bool BinMemory::atEndOf(Cache &cache, uint32_t addr) const {
    return cache != _blocks.end() && addr == cache->base + cache->data.size();
}

uint8_t BinMemory::readFrom(Cache &cache, uint32_t addr) const {
    return cache->data.at(addr - cache->base);
}

void BinMemory::appendTo(Cache &cache, uint32_t addr, uint8_t val) {
    cache->data.push_back(val);
    _writeCache = cache;
    aggregate(cache);
}

void BinMemory::replaceAt(Cache &cache, uint32_t addr, uint8_t val) {
    cache->data.at(addr - cache->base) = val;
    _writeCache = cache;
    aggregate(cache);
}

void BinMemory::createBlock(uint32_t addr, uint8_t val) {
    _writeCache = _blocks.emplace(addr).first;
    _writeCache->data.push_back(val);
    aggregate(_writeCache);
}

void BinMemory::aggregate(Cache &hint) {
    auto prev = hint;
    // Check following cache whether it is adjacent to.
    for (auto next = ++hint; next != _blocks.end() && atEndOf(prev, next->base);
            next = _blocks.erase(next)) {
        // Append next block.
        prev->data.reserve(prev->data.size() + next->data.size());
        prev->data.insert(prev->data.end(), std::make_move_iterator(next->data.begin()),
                std::make_move_iterator(next->data.end()));
        invalidateReadCache();
        invalidateWriteCache();
    }
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
