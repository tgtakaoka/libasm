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

#include "value.h"
#include "value_formatter.h"

namespace libasm {
namespace driver {

ListFormatter::ListFormatter()
    : _out(_outBuffer, sizeof(_outBuffer)), _rawFormatter(), _upperHex(true) {
    setListRadix(RADIX_16);
}

void ListFormatter::setUpperHex(bool enable) {
    _upperHex = enable;
}

void ListFormatter::setListRadix(Radix radix) {
    _listRadix = radix;
}

static uint8_t getDigits(uint32_t val, Radix radix) {
    const uint8_t base = uint8_t(radix);
    int8_t n = 0;
    do {
        n++;
        val /= base;
    } while (val);
    return n;
}

void ListFormatter::formatDec(uint32_t val, int8_t width) {
    if (width > 0) {
        const auto len = getDigits(val, RADIX_10);
        outSpaces(width - len);
    }
    const auto *start =_out.mark();
    StrCaseBuffer out{_out, _upperHex};
    _rawFormatter.formatDec(out, val, 32).over(_out);
    if (width < 0) {
        const auto len = _out.mark() - start;
        outSpaces(-width - len);
    }
}

static uint8_t getBits(const uint32_t val) {
    uint32_t mask = 0x80000000;
    uint8_t bits = 32;
    while (mask >= 2 && (val & mask) == 0) {
        mask >>= 1;
        bits--;
    }
    return bits;
}

static uint8_t getRadixBits(Radix radix) {
    return getBits(uint8_t(radix) - 1);
}

void ListFormatter::formatValue(uint32_t val, uint8_t bits, int8_t width, bool zeroSuppress) {
    if (bits == 0)
        bits = getBits(val);
    if (width > 0) {
        const auto radixBits = getRadixBits(_listRadix);
        const auto len = bits / radixBits + (bits % radixBits == 0 ? 0 : 1);
        outSpaces(width - len);
    }
    auto *start = _out.mark();
    StrCaseBuffer out{_out, _upperHex};
    if (_listRadix == RADIX_16) {
        _rawFormatter.formatHex(out, val, bits).over(_out);
    } else if (_listRadix == RADIX_8) {
        _rawFormatter.formatOct(out, val, bits).over(_out);
    }
    const auto *end = _out.mark();
    auto nonZero = start;
    if (zeroSuppress && *start == '0') {
        while (*nonZero == '0' && nonZero[1])
            nonZero++;
    }
    if (width < 0) {
        for (auto *p = start; p < end; ++p)
            *p = (nonZero < end) ? *nonZero++ : ' ';
        const uint8_t len = end - start;
        outSpaces(-width - len);
    }
}

void ListFormatter::formatAddress(uint32_t addr) {
    const auto width = addressColumnWidth() - 2;
    formatValue(addr, 0, width, true);
    _out.letter(' ').letter(':');
}

int ListFormatter::formatBytes(int base) {
    const auto unit = config().addressUnit();
    const auto width = config().opCodeWidth();
    const auto baseAddr = base + startAddress();
    const auto endian = config().endian();
    const auto generated = generatedSize();
    auto n = 0;
    for (auto pos = 0; base + n < generated && pos < bytesInLine(); pos++) {
        _out.letter(' ');
        uint16_t val = getByte(base + n++);
        if (width == OPCODE_16BIT && unit == ADDRESS_BYTE) {
            const auto digits = getDigits(0xFFFF, _listRadix);
            if (pos == 0 && baseAddr % 2 != 0) {  // the first byte
                if (endian == ENDIAN_BIG) {
                    formatValue(val, 8, digits);  // __HH
                } else {
                    formatValue(val, 8, -digits);  // HH__
                }
                continue;
            }
            if (base + n == generated) {  // the last byte
                if (endian == ENDIAN_BIG) {
                    formatValue(val, 8);  // HH
                } else {
                    formatValue(val, 8, digits);  // __HH
                }
                continue;
            }
        }
        if (width == OPCODE_8BIT) {
            formatValue(val, 8);
        } else if (width == OPCODE_16BIT) {
            const auto next8 = getByte(base + n++);
            val = endian == ENDIAN_BIG ? (val << 8) | next8 : val | (next8 << 8);
            formatValue(val, width);
            pos++;
        }
    }
    return n;
}

void ListFormatter::formatTab(int tabPosition, int delta) {
    auto pos = outLength();
    while (tabPosition < pos + 1)
        tabPosition += delta;
    outSpaces(tabPosition - pos);
    pos = tabPosition;
}

void ListFormatter::outSpaces(int n) {
    while (n > 0) {
        _out.letter(' ');
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
