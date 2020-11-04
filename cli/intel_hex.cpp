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

#include "bin_formatter.h"

#include <stdio.h>

namespace libasm {
namespace cli {

IntelHex::IntelHex(AddressWidth addrWidth)
    : BinFormatter(addrWidth),
      _ela(0)
{}

uint8_t IntelHex::getSum() const {
    return static_cast<uint8_t>(-this->_check_sum);
}

const char *IntelHex::begin() {
    _ela = 0;
    return nullptr;
}

// Output Type "04" Extended Linear Address, if necessary.
const char *IntelHex::prepare(uint32_t addr) {
    if (_addrWidth == ADDRESS_16BIT) return nullptr;
    const uint16_t ela = static_cast<uint16_t>(addr >> 16);
    if (_ela == ela) return nullptr;
    _ela = ela;
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
    char *p = _line;
    p += sprintf(p, ":%02X%04X%02X%04X%2X",
                 len, dummy, type, ela, getSum());
    return _line;
}

const char *IntelHex::encode(
    uint32_t ela_addr, const uint8_t *data, uint8_t size) {
    const uint16_t addr = static_cast<uint16_t>(ela_addr);
    const uint8_t type = 0;
    // :LLaaaa00dd....ddSS
    ensureLine(1 + (1 + sizeof(addr) + 1 + size + 1) * 2);
    char *p = _line;
    p += sprintf(p, ":%02X%04X%02X", static_cast<uint8_t>(size), addr, type);
    resetSum();
    addSum(static_cast<uint8_t>(size));
    addSum(addr);
    for (uint8_t i = 0; i < size; i++) {
        p += sprintf(p, "%02X", data[i]);
        addSum(data[i]);
    }
    sprintf(p, "%02X", getSum());
    return _line;
}

const char *IntelHex::end() { return ":00000001FF"; }

uint8_t *IntelHex::decode(
    const char *line, uint32_t &ela_addr, uint8_t &size) {
    ensureData(32);
    if (*line++ != ':') return nullptr;
    size = 0;
    uint8_t len = 0;
    if (parseByte(line, len)) return nullptr;
    uint16_t addr = 0;
    if (parseUint16(line, addr)) return nullptr;
    uint8_t type;
    if (parseByte(line, type)) return nullptr;
    if (type == 0x01) return _data; // terminator
    if (type == 0x04) {                   // Extended Linear Address
        _ela = addr;
        return _data;
    }
    if (type != 0x00) return nullptr;

    ela_addr = static_cast<uint32_t>(_ela) << 16 | addr;
    size = len;
    ensureData(size);
    resetSum();
    addSum(static_cast<uint8_t>(size));
    addSum(addr);
    for (uint8_t i = 0; i < size; i++) {
        if (parseByte(line, _data[i]))
            return nullptr;
    }
    uint8_t val = 0;
    if (parseByte(line, val)) return nullptr;
    if (_check_sum) return nullptr; // checksum error

    return _data;
}

} // namespace cli
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
