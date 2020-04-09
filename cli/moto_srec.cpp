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

MotoSrec::MotoSrec(AddressWidth addrWidth)
    : BinFormatter(addrWidth)
{}

uint8_t MotoSrec::getSum() const {
    return static_cast<uint8_t>(~_check_sum);
}

const char *MotoSrec::begin() {
    return "S0030000FC";
}

const char *MotoSrec::encode(
    uint32_t addr, const uint8_t *data, host::uint_t size) {
    uint8_t addrSize = 0;
    switch (_addrWidth) {
    case ADDRESS_16BIT: addrSize = 2; break;
    case ADDRESS_24BIT: addrSize = 3; break;
    case ADDRESS_32BIT: addrSize = 4; break;
    }
    ensureLine((addrSize + size + 3) * 2);
    const uint8_t len = addrSize + size + 1;
    resetSum();
    addSum(len);
    char *p = _line;
    switch (_addrWidth) {
    case ADDRESS_16BIT:
        addr &= ((uint32_t)1 << 16) - 1;
        p += sprintf(p, "S1%02X%04X", len, static_cast<uint16_t>(addr));
        break;
    case ADDRESS_24BIT:
        addr &= ((uint32_t)1 << 24) - 1;
        p += sprintf(p, "S2%02X%02X%04X", len,
                     static_cast<uint8_t>(addr >> 16),
                     static_cast<uint16_t>(addr));
        break;
    case ADDRESS_32BIT:
        p += sprintf(p, "S3%02X%08X", len,
                     static_cast<uint32_t>(addr));
        break;
    }
    addSum(addr);
    for (host::uint_t i = 0; i < size; i++) {
        p += sprintf(p, "%02X", data[i]);
        addSum(data[i]);
    }
    sprintf(p, "%02X", getSum());
    return _line;
}

const char *MotoSrec::end() {
    switch (_addrWidth) {
    case ADDRESS_16BIT:
        return "S9030000FC";
    case ADDRESS_24BIT:
        return "S804000000FB";
    case ADDRESS_32BIT:
        return "S70500000000FA";
    }
    return nullptr;
}

uint8_t *MotoSrec::decode(
    const char *line, uint32_t &addr, host::uint_t &size) {
    if (*line++ != 'S') return nullptr;
    const char type = *line++;
    ensureData(16);
    size = 0;
    if (type == '0')
        return _data; // start record
    if (type == '9' || type == '8' || type == '7')
        return _data; // end record
    if (type == '5' || type == '6')
        return _data; // record count
    if (type != '1' && type != '2' && type != '3')
        return nullptr;     // format error
    if (_addrWidth == ADDRESS_16BIT && (type == '2' || type == '3'))
        return nullptr;     // address size overflow
    if (_addrWidth == ADDRESS_24BIT && type == '3')
        return nullptr;     // address size overflow
    resetSum();
    uint8_t len = 0;
    if (parseByte(line, len)) return nullptr;
    uint16_t val16 = 0;
    if (type == '1') {
        if (len < 2) return nullptr;
        if (parseUint16(line, val16)) return nullptr;
        addr = val16;
        len -= 2;
    }
    if (type == '2') {
        if (len < 3) return nullptr;
        uint8_t val8;
        if (parseByte(line, val8)) return nullptr;
        addr = static_cast<uint32_t>(val8) << 16;
        if (parseUint16(line, val16)) return nullptr;
        addr |= val16;
        len -= 3;
    }
    if (type == '3') {
        if (len < 4) return nullptr;
        if (parseUint16(line, val16)) return nullptr;
        addr = static_cast<uint32_t>(val16) << 16;
        if (parseUint16(line, val16)) return nullptr;
        addr |= val16;
        len -= 4;
    }
    if (len < 1) return nullptr;
    size = len - 1;

    ensureData(size);
    for (host::uint_t i = 0; i < size; i++)
        if (parseByte(line, _data[i])) return nullptr;
    const uint8_t sum = getSum();
    uint8_t val = 0;
    if (parseByte(line, val)) return nullptr;
    if (val != sum)             // checksum error
        return nullptr;

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
