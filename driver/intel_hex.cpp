/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "intel_hex.h"

namespace libasm {
namespace driver {

static IntelHex INSTANCE;

BinDecoder &IntelHex::decoder() {
    return INSTANCE;
}

BinEncoder &IntelHex::encoder() {
    return INSTANCE;
}

uint8_t IntelHex::getSum() const {
    return static_cast<uint8_t>(-_check_sum);
}

void IntelHex::begin(TextPrinter &out) {
    reset();
}

void IntelHex::encode(TextPrinter &out, uint32_t addr, const uint8_t *data, uint_fast8_t size) {
    // If |addr| has different ELA than last
    if ((addr & ~UINT16_MAX) != 0 && ((addr ^ _last_addr) & ~UINT16_MAX) != 0) {
        formatEla(out, addr);
    }
    const auto end = addr + size;
    // If this block overwarp ELA boundary.
    if ((addr ^ end) & ~UINT16_MAX) {
        const auto chunk = (addr & ~UINT16_MAX) + UINT32_C(0x10000) - addr;
        encodeLine(out, addr, data, chunk);
        _last_addr = addr;
        addr += chunk;
        _next_addr = addr;
        data += chunk;
        size -= chunk;
        if (size == 0)
            return;
    }
    encodeLine(out, addr, data, size);
    _last_addr = addr;
    _next_addr = addr + size;
}

// Output Type "04" Extended Linear Address.
void IntelHex::formatEla(TextPrinter &out, uint32_t addr) {
    if (addressSize(addr) <= 2)
        return;
    const auto ela = static_cast<uint16_t>(addr >> 16);
    const auto len = sizeof(ela);
    const auto dummy = 0;
    const auto type = 4;
    // :LLdddd04EEEESS
    resetSum();
    addSum8(len);
    addSum16(dummy);
    addSum8(type);
    addSum16(ela);
    out.format(":%02X%04X%02X%04X%2X\n", len, dummy, type, ela, getSum());
}

void IntelHex::encodeLine(TextPrinter &out, uint16_t addr, const uint8_t *data, uint_fast8_t size) {
    const auto type = 0;
    resetSum();
    addSum8(size);
    addSum16(addr);
    addSum8(type);
    // :LLaaaa00dd....ddSS
    out.format(":%02X%04X%02X", size, addr, type);
    for (uint_fast8_t i = 0; i < size; i++) {
        addSum8(data[i]);
        out.format("%02X", data[i]);
    }
    out.format("%02X\n", getSum());
}

void IntelHex::end(TextPrinter &out) {
    out.println(":00000001FF");
}

void IntelHex::reset() {
    _last_addr = 0;
    _next_addr = 0;
}

int IntelHex::decode(StrScanner &line, BinMemory &memory) {
    if (!line.expect(':'))
        return -1;
    resetSum();
    uint8_t len = 0;
    if (!parseByte(line, len))
        return -1;
    addSum8(len);
    uint16_t addr = 0;
    if (!parseUint16(line, addr))
        return -1;
    addSum16(addr);
    uint8_t type;
    if (!parseByte(line, type))
        return -1;
    addSum8(type);
    if (type == 0x04) {  // Extended Linear Address
        uint16_t ela = 0;
        if (!parseUint16(line, ela))
            return -1;
        addSum16(ela);
        _next_addr = static_cast<uint32_t>(ela) << 16;
        uint8_t sum = 0;
        if (!parseByte(line, sum))
            return -1;
        addSum8(sum);
        if (_check_sum)
            return -1;  // checksum error
        return 0;
    }
    if (type != 0x00)
        return 0;  // ignore 01, 03, 05 record

    const uint32_t ela_addr = _next_addr | addr;
    for (uint8_t i = 0; i < len; i++) {
        uint8_t data = 0;
        if (!parseByte(line, data))
            return -1;
        addSum8(data);
        memory.writeByte(ela_addr + i, data);
    }
    uint8_t sum = 0;
    if (!parseByte(line, sum))
        return -1;
    addSum8(sum);
    if (_check_sum)
        return -1;  // checksum error

    return len;
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
