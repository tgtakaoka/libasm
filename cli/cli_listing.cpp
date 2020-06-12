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

#include "cli_listing.h"

#include <string>

namespace libasm {
namespace cli {

void CliListing::reset(ListingLine &line) {
    _line = &line;
    _next = 0;
}

bool CliListing::hasNext() const {
    return _next < _line->generatedSize();
}

const char *CliListing::getContent() {
    _out.clear();
    formatContent(0);
    _next = _line->generatedSize();
    return _out.c_str();
}

const char *CliListing::getLine() {
    _out.clear();
    formatLine();
    return _out.c_str();
}

void CliListing::formatHex(uint8_t val) {
    val &= 0xf;
    if (val < 10) {
        _out += '0' + val;
    } else if (_uppercase) {
        _out += 'A' + val - 10;
    } else {
        _out += 'a' + val - 10;
    }
}

void CliListing::formatUint8(
    uint8_t val, bool fixedWidth, bool zeroSuppress) {
    const uint8_t msd = (val >> 4);
    if (msd || !zeroSuppress) {
        formatHex(msd);
    } else if (fixedWidth) {
        _out += ' ';
    }
    formatHex(val);
}

void CliListing::formatUint16(
    uint16_t val, bool fixedWidth, bool zeroSuppress) {
    const uint8_t msb = static_cast<uint8_t>(val >> 8);
    if (msb || !zeroSuppress) {
        formatUint8(msb, fixedWidth, zeroSuppress);
        zeroSuppress = false;
    } else if (fixedWidth) {
        _out += "  ";
    }
    formatUint8(static_cast<uint8_t>(val), fixedWidth, zeroSuppress);
}

void CliListing::formatUint24(
    uint32_t val, bool fixedWidth, bool zeroSuppress) {
    const uint8_t msb = static_cast<uint8_t>(val >> 16);
    if (msb || !zeroSuppress) {
        formatUint8(msb, fixedWidth, zeroSuppress);
        zeroSuppress = false;
    } else if (fixedWidth) {
        _out += "  ";
    }
    formatUint16(static_cast<uint16_t>(val), fixedWidth, zeroSuppress);
}

void CliListing::formatUint32(
    uint32_t val, bool fixedWidth, bool zeroSuppress) {
    const uint16_t msw = static_cast<uint16_t>(val >> 16);
    if (msw || !zeroSuppress) {
        formatUint16(msw, fixedWidth, zeroSuppress);
        zeroSuppress = false;
    } else if (fixedWidth) {
        _out += "    ";
    }
    formatUint16(static_cast<uint16_t>(val), fixedWidth, zeroSuppress);
}

void CliListing::formatAddress(
    uint32_t addr, bool fixedWidth, bool zeroSuppress) {
    switch (_line->addressWidth()) {
    case ADDRESS_16BIT:
        formatUint16(addr, fixedWidth, zeroSuppress);
        break;
    case ADDRESS_24BIT:
        formatUint24(addr, fixedWidth, zeroSuppress);
        break;
    case ADDRESS_32BIT:
        formatUint32(addr, fixedWidth, zeroSuppress);
        break;
    }
    _out += ':';
}

int CliListing::formatBytes(int base) {
    const int maxBytes = _line->maxBytes();
    int i = 0;
    while (base + i < _line->generatedSize() && i < maxBytes) {
        uint8_t val = _line->getByte(base + i);
        switch (_line->opCodeWidth()) {
        case OPCODE_8BIT:
            _out += ' ';
            break;
        case OPCODE_16BIT:
            if (i % 2 == 0)
                _out += ' ';
            break;
        }
        formatUint8(val);
        i++;
    }
    return i;
}

void CliListing::formatTab(size_t pos, int delta) {
    while (pos < _out.size() + 1)
        pos += delta;
    while (_out.size() < pos)
        _out += ' ';
}

static void convertCase(const std::string &src, std::string &out, bool uppercase) {
    for (char c : src) {
        if (uppercase) c = toupper(c);
        else c = tolower(c);
        out += c;
    }
}

void CliListing::formatContent(int pos) {
    if (!_line->hasLabel() && !_line->hasInstruction() && _line->hasComment()) {
        formatTab(pos);
        _out += _line->getComment();
        return;
    }
    if (_line->hasLabel()) {
        formatTab(pos);
        _out += _line->getLabel();
    }
    if (_line->hasInstruction()) {
        formatTab(pos + _line->labelWidth(), 8);
        convertCase(_line->getInstruction(), _out, _uppercase);
    }
    if (_line->hasOperand()) {
        formatTab(pos + _line->labelWidth()
                  + _line->instructionWidth());
        _out += _line->getOperand();
    }
    if (_line->hasComment()) {
        formatTab(pos + _line->labelWidth()
                  + _line->instructionWidth() + _line->operandWidth());
        _out += _line->getComment();
    }
}

void CliListing::formatLine() {
    if (_lineNumber) {
        char buf[12];
        uint16_t nest = _line->includeNest();
        if (nest) {
            sprintf(buf, "(%d)", nest);
            _out += buf;
        } else {
            _out += "   ";
        }
        sprintf(buf, "%5d/ ", _line->lineNumber());
        _out += buf;
    }
    formatAddress(_line->startAddress() + _next, true, true);
    const int pos = _out.size();
    int formattedBytes = 0;
    if (_line->hasValue()) {
        _out += " =$";
        formatUint32(_line->value(), false, true);
        formattedBytes = 0;
    } else {
        formattedBytes = formatBytes(_next);
    }
    const int dataTextLen = _line->opCodeWidth() == OPCODE_8BIT
        ? (_line->maxBytes() * 3)
        : (_line->maxBytes() / 2) * 5;
    if (_next == 0) formatContent(pos + dataTextLen + 1);
    _next += formattedBytes;
}

} // namespace cli
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
