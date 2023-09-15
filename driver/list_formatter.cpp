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

ListFormatter::ListFormatter()
    : _formatter(), _out(_out_buffer, sizeof(_out_buffer)), _upperHex(true) {}

void ListFormatter::setUpperHex(bool enable) {
    _upperHex = enable;
}

void ListFormatter::formatDec(uint32_t val, int8_t width) {
    if (width > 0) {
        auto len = 1;
        for (auto v = val; v >= 10; v /= 10)
            len++;
        for (; len < width; len++)
            _out.letter(' ');
    }
    auto start = _out.mark();
    _formatter.formatDec(_out, val, 32);
    if (width < 0) {
        width = -width;
        for (auto len = _out.mark() - start; len < width; len++)
            _out.letter(' ');
    }
}

void ListFormatter::formatHex(uint32_t val, uint8_t bits, bool zeroSuppress) {
    if (zeroSuppress && bits == 0) {
        bits = 32;
        uint32_t mask = 0x80000000;
        while (mask >= 2 && (val & mask) == 0) {
            mask >>= 1;
            bits--;
        }
    }
    auto start = _out.mark();
    _formatter.formatHex(_out, val, bits, _upperHex, false);
    if (zeroSuppress) {
        while (*start == '0' && start[1])
            *start++ = ' ';
    }
}

void ListFormatter::formatAddress(uint32_t addr, bool fixedWidth) {
    formatHex(addr, fixedWidth ? ADDRESS_32BIT : 0, true);
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
            if (pos == 0 && baseAddr % 2 != 0) { // the first byte
                if (endian == ENDIAN_BIG) {
                    _out.letter(' ').letter(' ');
                    formatHex(val, 8);  // __HH
                } else {
                    formatHex(val, 8);
                    _out.letter(' ').letter(' ');  // HH__
                }
                continue;
            }
            if (base + n == generated) { // the last byte
                if (endian == ENDIAN_BIG) {
                    formatHex(val, 8);  // HH
                } else {
                    _out.letter(' ').letter(' ');  // __HH
                    formatHex(val, 8);
                }
                continue;
            }
        }
        if (width == OPCODE_16BIT) {
            const auto next8 = getByte(base + n++);
            val = endian == ENDIAN_BIG ? (val << 8) | next8 : val | (next8 << 8);
            formatHex(val, 16);
            pos++;
        } else {
            formatHex(val, 8);
        }
    }
    return n;
}

void ListFormatter::formatTab(int tabPosition, int delta) {
    auto pos = outLength();
    while (tabPosition < pos + 1)
        tabPosition += delta;
    while (pos < tabPosition) {
        _out.letter(' ');
        pos++;
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
