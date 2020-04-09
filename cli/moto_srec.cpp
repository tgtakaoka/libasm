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

MotoSrec::MotoSrec(host::uint_t addrWidth)
    : BinFormatter(addrWidth)
{}

const char *MotoSrec::begin() {
    return "S0030000FC";
}

const char *MotoSrec::encode(
    uint32_t addr, const uint8_t *data, host::uint_t size) {
    ensureLine((_addrWidth + size + 3) * 2);
    const uint8_t len = _addrWidth + size + 1;
    this->resetSum();
    this->addSum(len);
    char *p = this->_line;
    if (_addrWidth == 2) {
        p += sprintf(p, "S1%02X%04X", len,
                     static_cast<uint16_t>(addr));
    }
    if (_addrWidth == 3) {
        p += sprintf(p, "S2%02X%02X%04X", len,
                     static_cast<uint8_t>(addr >> 16),
                     static_cast<uint16_t>(addr));
    }
    if (_addrWidth == 4) {
        p += sprintf(p, "S3%02X%08X", len,
                     static_cast<uint32_t>(addr));
    }
    this->addSum(addr);
    for (host::uint_t i = 0; i < size; i++) {
        p += sprintf(p, "%02X", data[i]);
        this->addSum(data[i]);
    }
    sprintf(p, "%02X", static_cast<uint8_t>(~this->_check_sum));
    return this->_line;
}

const char *MotoSrec::end() {
    if (_addrWidth == 2)
        return "S9030000FC";
    if (_addrWidth == 3)
        return "S8030000FC";
    if (_addrWidth == 4)
        return "S70500000000FA";
    return nullptr;
}

uint8_t *MotoSrec::decode(
    const char *line, uint32_t &addr, host::uint_t &size) {
    if (*line++ != 'S') return nullptr;
    const char type = *line++;
    this->ensureData(16);
    size = 0;
    if (type == '0')
        return this->_data; // start record
    if (type == '9' || type == '8' || type == '7')
        return this->_data; // end record
    if (type == '5' || type == '6')
        return this->_data; // record count
    if (type != '1' && type != '2' && type != '3')
        return nullptr;     // format error
    if (_addrWidth == 2 && (type == '2' || type == '3'))
        return nullptr;     // address size overflow
    if (_addrWidth == 3 && type == '3')
        return nullptr;     // address size overflow
    this->resetSum();
    uint8_t len = 0;
    if (this->parseByte(line, len)) return nullptr;
    uint16_t val16 = 0;
    if (type == '1') {
        if (len < 2) return nullptr;
        if (this->parseUint16(line, val16)) return nullptr;
        addr = val16;
        len -= 2;
    }
    if (type == '2') {
        if (len < 3) return nullptr;
        uint8_t val8;
        if (this->parseByte(line, val8)) return nullptr;
        addr = static_cast<uint32_t>(val8) << 16;
        if (this->parseUint16(line, val16)) return nullptr;
        addr |= val16;
        len -= 3;
    }
    if (type == '3') {
        if (len < 4) return nullptr;
        if (this->parseUint16(line, val16)) return nullptr;
        addr = static_cast<uint32_t>(val16) << 16;
        if (this->parseUint16(line, val16)) return nullptr;
        addr |= val16;
        len -= 4;
    }
    if (len < 1) return nullptr;
    size = len - 1;

    this->ensureData(size);
    for (host::uint_t i = 0; i < size; i++)
        if (this->parseByte(line, this->_data[i])) return nullptr;
    const uint8_t sum = this->_check_sum;
    uint8_t val = 0;
    if (this->parseByte(line, val)) return nullptr;
    if (static_cast<uint8_t>(~val) != sum) // checksum error
        return nullptr;

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
