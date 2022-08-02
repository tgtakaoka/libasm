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
    : _formatter(/*cstyle*/ false), _out(_out_buffer, sizeof(_out_buffer)) {
    _formatter.setUppercase(true);
}

void ListFormatter::setUpperHex(bool enable) {
    _formatter.setUppercase(enable);
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
    _formatter.formatHex(_out, val, bits, false);
    if (zeroSuppress) {
        while (*start == '0' && start[1])
            *start++ = ' ';
    }
}

void ListFormatter::formatAddress(uint32_t addr, bool fixedWidth) {
    formatHex(addr, fixedWidth ? uint8_t(config().addressWidth()) : 0, true);
    _out.letter(':');
}

int ListFormatter::bytesColumnWidth() const {
    const auto numBytes = bytesInLine();
    if (config().opCodeWidth() == OPCODE_8BIT) {
        return numBytes * 3;  // "HH "
    } else {
        return (numBytes / 2) * 5;  // "HHHH "
    }
}

int ListFormatter::formatBytes(int base) {
    const auto generated = generatedSize();
    const auto bytes = bytesInLine();
    int i = 0;
    if (config().opCodeWidth() == OPCODE_8BIT) {
        while (base + i < generated && i < bytes) {
            const uint8_t val8 = getByte(base + i++);
            _out.letter(' ');
            formatHex(val8, 8);
        }
    } else {  // OPCODE_16BIT
        while (base + i < generated && i < bytes) {
            uint16_t val16 = getByte(base + i++);
            const uint16_t next8 = getByte(base + i++);
            if (config().endian() == ENDIAN_BIG) {
                val16 = (val16 << 8) | next8;
            } else {
                val16 |= (next8 << 8);
            }
            _out.letter(' ');
            formatHex(val16, 16);
        }
    }
    return i;
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
