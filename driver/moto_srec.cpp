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

#include "moto_srec.h"

namespace libasm {
namespace driver {

static MotoSrec INSTANCE;

BinDecoder &MotoSrec::decoder() {
    return INSTANCE;
}

BinEncoder &MotoSrec::encoder() {
    return INSTANCE;
}

uint8_t MotoSrec::getSum() const {
    return static_cast<uint8_t>(~_check_sum);
}

void MotoSrec::begin(TextPrinter &out) {
    out.println("S0030000FC");
}

void MotoSrec::encode(TextPrinter &out, uint32_t addr, const uint8_t *data, uint8_t size) {
    resetSum();
    const auto addr_size = addressSize(_addr_width);
    const uint8_t len = addr_size + size + 1;
    addSum8(len);
    switch (addr_size) {
    case 2:
        out.format("S1%02X%04X", len, static_cast<uint16_t>(addr));
        addSum16(addr);
        break;
    case 3:
        out.format("S2%02X%02X%04X", len, static_cast<uint8_t>(addr >> 16),
                static_cast<uint16_t>(addr));
        addSum24(addr);
        break;
    default:
        out.format("S3%02X%08X", len, static_cast<uint32_t>(addr));
        addSum32(addr);
        break;
    }
    for (uint8_t i = 0; i < size; i++) {
        out.format("%02X", data[i]);
        addSum8(data[i]);
    }
    out.format("%02X\n", getSum());
}

void MotoSrec::end(TextPrinter &out) {
    switch (addressSize(_addr_width)) {
    case 2:
        out.println("S9030000FC");
        break;
    case 3:
        out.println("S804000000FB");
        break;
    default:
        out.println("S70500000000FA");
    }
}

int MotoSrec::decode(StrScanner &line, BinMemory &memory) {
    if (!line.expect('S'))
        return -1;
    const char type = *line++;
    if (type == '0')
        return 0;  // start record
    if (type == '9' || type == '8' || type == '7')
        return 0;  // end record
    if (type == '5' || type == '6')
        return 0;  // record count
    if (type != '1' && type != '2' && type != '3')
        return -1;  // format error
    resetSum();
    uint8_t len = 0;
    if (!parseByte(line, len))
        return -1;
    addSum8(len);
    uint32_t addr = 0;
    uint16_t val16 = 0;
    if (type == '1') {
        if (len < 2 || !parseUint16(line, val16))
            return -1;
        addSum16(val16);
        addr = val16;
        len -= 2;
    }
    if (type == '2') {
        if (len < 3 || !parseUint24(line, addr))
            return -1;
        addSum24(addr);
        len -= 3;
    }
    if (type == '3') {
        if (len < 4 || !parseUint32(line, addr))
            return -1;
        addSum32(addr);
        len -= 4;
    }
    if (len < 1)
        return -2;
    const auto size = len - 1;
    for (uint8_t i = 0; i < size; i++) {
        uint8_t data = 0;
        if (!parseByte(line, data))
            return -1;
        addSum8(data);
        memory.writeByte(addr + i, data);
    }
    const uint8_t sum = getSum();
    uint8_t val8 = 0;
    if (!parseByte(line, val8))
        return -1;
    if (val8 != sum)  // checksum error
        return -1;

    return size;
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
