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

#include <stdio.h>

#include "bin_formatter.h"

namespace libasm {
namespace cli {

IntelHex::IntelHex(AddressWidth addrWidth)
    : BinFormatter(addrWidth), _last_addr(-1), _next_addr(0) {}

uint8_t IntelHex::getSum() const {
    return static_cast<uint8_t>(-this->_check_sum);
}

void IntelHex::begin(FILE *out) {
    _out = out;
    _last_addr = -1;
    _next_addr = 0;
}

void IntelHex::encode(uint32_t addr, const uint8_t *data, uint8_t size) {
    const uint32_t end = addr + size;
    // If this block overwarp ELA boundary.
    if ((addr ^ end) & ~0xFFFF) {
        const uint8_t chunk = (end & ~0xFFFF) - addr;
        encodeLine(addr, data, chunk);
        _last_addr = addr;
        addr += chunk;
        _next_addr = addr;
        data += chunk;
        size -= chunk;
        if (size == 0)
            return;
    }
    // If |addr| is discontinued or |addr| has different ELA than last
    if (addr != _next_addr || ((addr ^ _last_addr) >> 16) != 0)
        formatEla(addr);
    encodeLine(addr, data, size);
    _last_addr = addr;
    _next_addr = addr + size;
}

// Output Type "04" Extended Linear Address.
void IntelHex::formatEla(uint32_t addr) {
    if (_addrSize <= 2)
        return;
    const uint16_t ela = static_cast<uint16_t>(addr >> 16);
    const uint8_t len = sizeof(ela);
    const uint16_t dummy = 0;
    const uint8_t type = 4;
    // :LLdddd04EEEESS
    ensureLine(1 + (1 + sizeof(dummy) + 1 + len + 1) * 2);
    resetSum();
    addSum(len);
    addSum(dummy);
    addSum(type);
    addSum(ela);
    sprintf(_line, ":%02X%04X%02X%04X%2X", len, dummy, type, ela, getSum());
    format(_line);
}

void IntelHex::encodeLine(uint16_t addr, const uint8_t *data, uint8_t size) {
    const uint8_t type = 0;
    // :LLaaaa00dd....ddSS
    ensureLine(1 + (1 + sizeof(addr) + 1 + size + 1) * 2);
    char *p = _line;
    p += sprintf(p, ":%02X%04X%02X", size, addr, type);
    resetSum();
    addSum(size);
    addSum(addr);
    for (uint8_t i = 0; i < size; i++) {
        p += sprintf(p, "%02X", data[i]);
        addSum(data[i]);
    }
    sprintf(p, "%02X", getSum());
    format(_line);
}

void IntelHex::end() {
    format(":00000001FF");
}

uint8_t *IntelHex::decode(const char *line, uint32_t &ela_addr, uint8_t &size) {
    if (*line++ != ':')
        return nullptr;
    ensureData(16);
    size = 0;
    resetSum();
    uint8_t len = 0;
    if (parseByte(line, len))
        return nullptr;
    uint16_t addr = 0;
    if (parseUint16(line, addr))
        return nullptr;
    uint8_t type;
    if (parseByte(line, type))
        return nullptr;
    if (type == 0x01)
        return _data;    // terminator
    if (type == 0x04) {  // Extended Linear Address
        uint16_t ela = 0;
        if (parseUint16(line, ela))
            return nullptr;
        _next_addr = static_cast<uint32_t>(ela) << 16;
        uint8_t sum = 0;
        if (parseByte(line, sum))
            return nullptr;
        if (_check_sum)
            return nullptr;  // checksum error
        return _data;
    }
    if (type != 0x00)
        return nullptr;

    ela_addr = _next_addr | addr;
    size = len;
    ensureData(size);
    for (uint8_t i = 0; i < size; i++) {
        if (parseByte(line, _data[i]))
            return nullptr;
    }
    uint8_t sum = 0;
    if (parseByte(line, sum))
        return nullptr;
    if (_check_sum)
        return nullptr;  // checksum error

    return _data;
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
