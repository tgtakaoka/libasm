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

IntelHex::IntelHex(host::uint_t addrWidth)
    : BinFormatter(addrWidth)
{}

const char *IntelHex::begin() {
    return nullptr;
}

const char *IntelHex::encode(
    uint32_t addr, const uint8_t *data, host::uint_t size) {
    ensureLine((_addrWidth + size + 3) * 2);
    char *p = this->_line;
    p += sprintf(p, ":%02X%04X00",
                 static_cast<uint8_t>(size),
                 static_cast<uint16_t>(addr));
    this->resetSum();
    this->addSum(static_cast<uint8_t>(size));
    this->addSum(addr);
    for (host::uint_t i = 0; i < size; i++) {
        p += sprintf(p, "%02X", data[i]);
        this->addSum(data[i]);
    }
    sprintf(p, "%02X", static_cast<uint8_t>(-this->_check_sum));
    return this->_line;
}

const char *IntelHex::end() { return ":00000001FF"; }

uint8_t *IntelHex::decode(
    const char *line, uint32_t &addr, host::uint_t &size) {
    if (*line++ != ':') return nullptr;
    size = 0;
    uint8_t len = 0;
    if (this->parseByte(line, len)) return nullptr;
    uint16_t val16 = 0;
    if (this->parseUint16(line, val16)) return nullptr;
    addr = val16;
    uint8_t type;
    if (this->parseByte(line, type)) return nullptr;
    if (type == 0x01) return this->_data; // terminator
    if (type != 0x00) return nullptr;

    size = len;
    this->ensureData(size);
    this->resetSum();
    this->addSum(static_cast<uint8_t>(size));
    this->addSum(addr);
    for (host::uint_t i = 0; i < size; i++) {
        if (this->parseByte(line, this->_data[i]))
            return nullptr;
    }
    uint8_t val = 0;
    if (this->parseByte(line, val)) return nullptr;
    if (this->_check_sum) return nullptr; // checksum error

    return this->_data;
}

} // namespace cli
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
