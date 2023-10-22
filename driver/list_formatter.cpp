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

#include "list_formatter.h"

#include <string>

namespace libasm {
namespace driver {

ListFormatter::ListFormatter(const Provider &provider)
    : _provider(provider), _formatter(), _upperHex(true), _listRadix(RADIX_16) {}

void ListFormatter::setUpperHex(bool enable) {
    _upperHex = enable;
}

void ListFormatter::setListRadix(Radix radix) {
    _listRadix = radix;
}

static uint8_t calcDigits(uint32_t val, Radix radix) {
    const uint8_t base = uint8_t(radix);
    int8_t n = 0;
    do {
        n++;
        val /= base;
    } while (val);
    return n;
}

void ListFormatter::formatDec(StrBuffer &out, uint32_t val, int8_t width) {
    if (width > 0) {
        const auto len = calcDigits(val, RADIX_10);
        outSpaces(out, width - len);
    }
    const auto *start = out.mark();
    _formatter.formatDec(out, val, 32);
    if (width < 0) {
        const auto len = out.mark() - start;
        outSpaces(out, -width - len);
    }
}

static uint8_t calcBits(const uint32_t val) {
    uint32_t mask = 0x80000000;
    uint8_t bits = 32;
    while (mask >= 2 && (val & mask) == 0) {
        mask >>= 1;
        bits--;
    }
    return bits;
}

static uint8_t calcRadixBits(Radix radix) {
    return calcBits(uint8_t(radix) - 1);
}

void ListFormatter::formatValue(StrBuffer &out, uint32_t val, uint8_t bits, int8_t width, bool zeroSuppress) {
    if (bits == 0)
        bits = calcBits(val);
    if (width > 0) {
        const auto radixBits = calcRadixBits(_listRadix);
        const auto len = bits / radixBits + (bits % radixBits == 0 ? 0 : 1);
        outSpaces(out, width - len);
    }
    auto *start = out.mark();
    if (_listRadix == RADIX_16) {
        StrCaseBuffer hexOut{out, _upperHex};
        _formatter.formatHex(hexOut, val, bits).over(out);
    } else if (_listRadix == RADIX_8) {
        _formatter.formatOct(out, val, bits);
    }
    const auto *end = out.mark();
    auto nonZero = start;
    if (zeroSuppress && *start == '0') {
        while (*nonZero == '0' && nonZero[1])
            nonZero++;
    }
    if (width < 0) {
        for (auto *p = start; p < end; ++p)
            *p = (nonZero < end) ? *nonZero++ : ' ';
        const uint8_t len = end - start;
        outSpaces(out, -width - len);
    }
}

void ListFormatter::formatAddress(StrBuffer &out, uint32_t addr) {
    const auto width = addressColumnWidth() - 2;
    formatValue(out, addr, 0, width, true);
    out.letter(' ').letter(':');
}

uint8_t ListFormatter::formatBytes(StrBuffer &out, uint8_t offset) {
    const auto &config = _provider.config();
    const auto unit = config.addressUnit();
    const auto width = config.opCodeWidth();
    const auto endian = config.endian();
    const auto addr = offset + _provider.startAddress();
    const auto size = _provider.bytesSize();
    uint8_t len = 0;
    for (auto pos = 0; offset + len < size && pos < bytesInLine(); pos++) {
        out.letter(' ');
        uint16_t val = _provider.getByte(offset + len++);
        if (width == OPCODE_16BIT && unit == ADDRESS_BYTE) {
            const auto digits = calcDigits(0xFFFF, _listRadix);
            if (pos == 0 && addr % 2 != 0) {  // the first byte
                if (endian == ENDIAN_BIG) {
                    formatValue(out, val, 8, digits);  // __HH
                } else {
                    formatValue(out, val, 8, -digits);  // HH__
                }
                continue;
            }
            if (offset + len == size) {  // the last byte
                if (endian == ENDIAN_BIG) {
                    formatValue(out, val, 8);  // HH
                } else {
                    formatValue(out, val, 8, digits);  // __HH
                }
                continue;
            }
        }
        if (width == OPCODE_8BIT) {
            formatValue(out, val, 8);
        } else if (width == OPCODE_16BIT || width == OPCODE_12BIT) {
            const auto next8 = _provider.getByte(offset + len++);
            val = endian == ENDIAN_BIG ? (val << 8) | next8 : val | (next8 << 8);
            formatValue(out, val, width);
            pos++;
        }
    }
    return len;
}

void ListFormatter::formatTab(StrBuffer &out, int tabPosition, int delta) {
    auto pos = out.len();
    while (tabPosition < pos + 1)
        tabPosition += delta;
    outSpaces(out, tabPosition - pos);
    pos = tabPosition;
}

void ListFormatter::outSpaces(StrBuffer &out, int n) {
    while (n > 0) {
        out.letter(' ');
        n--;
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
