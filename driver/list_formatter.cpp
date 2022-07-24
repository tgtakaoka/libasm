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

void ListFormatter::reset(ListLine &line) {
    _line = &line;
    _nextContent = _nextLine = 0;
    _errorContent = _errorLine = false;
}

void ListFormatter::setUppercase(bool uppercase) {
    _formatter.setUppercase(_uppercase = uppercase);
}

bool ListFormatter::hasNextContent() const {
    return _nextContent < _line->generatedSize();
}

bool ListFormatter::hasNextLine() const {
    return _nextLine < _line->generatedSize();
}

const char *ListFormatter::getContent() {
    _out.reset(_outBuffer, sizeof(_outBuffer));
    if (_line->isError()) {
        if (!_errorContent) {
            if (_line->lineNumber()) {
                // TODO: assembler error
            } else {
                // disassembler error
                _out.text("; ").text(_line->inputName().c_str()).text(": 0x");
                formatAddress(_line->startAddress(), true);
                _out.letter(' ').text(_line->errorText().c_str());
                _nextContent = 0;
            }
            _errorContent = true;
        } else {
            if (_line->lineNumber()) {
                // TODO: assembler error
            } else {
                _out.text("; ");
                formatAddress(_line->startAddress(), true);
                _nextContent += formatBytes(_nextContent);
            }
        }
    } else {
        formatContent(0);
        _nextContent = _line->generatedSize();
    }
    return _outBuffer;
}

const char *ListFormatter::getLine() {
    _out.reset(_outBuffer, sizeof(_outBuffer));
    if (_line->isError() && !_errorLine) {
        if (_line->lineNumber()) {
            // TODO: assembler error
        } else {
            // disassembler error
            _out.text(_line->inputName().c_str()).text(": 0x");
            formatAddress(_line->startAddress(), true);
            _out.letter(' ').text(_line->errorText().c_str());
            _nextLine = 0;
        }
        _errorLine = true;
    } else {
        formatLine();
    }
    return _outBuffer;
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

void ListFormatter::formatAddress(uint32_t addr, bool fixedWidth, bool zeroSuppress) {
    formatHex(addr, fixedWidth ? uint8_t(_line->config().addressWidth()) : 0, zeroSuppress);
    _out.letter(':');
}

int ListFormatter::formatBytes(int base) {
    const auto generated = _line->generatedSize();
    const auto codeBytes = _line->codeBytes();
    int i = 0;
    if (_line->config().opCodeWidth() == OPCODE_8BIT) {
        while (base + i < generated && i < codeBytes) {
            const uint8_t val8 = _line->getByte(base + i);
            _out.letter(' ');
            formatHex(val8, 8);
            i++;
        }
    } else {  // OPCODE_16BIT
        const int hi = int(_line->config().endian());
        const int lo = 1 - hi;
        while (base + i < generated && i < codeBytes) {
            const uint8_t val8hi = _line->getByte(base + i + hi);
            const uint8_t val8lo = _line->getByte(base + i + lo);
            const uint16_t val16 = (static_cast<uint16_t>(val8hi) << 8) | val8lo;
            _out.letter(' ');
            formatHex(val16, 16);
            i += 2;
        }
    }
    return i;
}

void ListFormatter::formatTab(size_t pos, int delta) {
    size_t size = _out.mark() - _outBuffer;
    while (pos < size + 1)
        pos += delta;
    while (size < pos) {
        _out.letter(' ');
        size++;
    }
}

void ListFormatter::formatContent(int pos) {
    if (!_line->hasLabel() && !_line->hasInstruction() && _line->hasComment()) {
        formatTab(pos);
        _out.text(_line->getComment().c_str());
        return;
    }
    if (_line->hasLabel()) {
        formatTab(pos);
        _out.text(_line->getLabel().c_str());
    }
    pos += _line->labelWidth();
    if (_line->hasInstruction()) {
        formatTab(pos, 8);
        _out.text(_line->getInstruction().c_str(), _uppercase);
    }
    pos += _line->nameWidth();
    if (_line->hasOperand()) {
        formatTab(pos);
        _out.text(_line->getOperand().c_str());
    }
    pos += _line->operandWidth();
    if (_line->hasComment()) {
        formatTab(pos);
        _out.text(_line->getComment().c_str());
    }
}

void ListFormatter::formatLine() {
    if (_lineNumber) {
        uint16_t nest = _line->includeNest();
        if (nest) {
            _formatter.formatDec(_out, nest, 0);
        } else {
            _out.text("   ");
        }
        // TODO: fixed 5 letter width
        _formatter.formatDec(_out, _line->lineNumber(), 0);
        _out.text("/ ");
    }
    formatAddress(_line->startAddress() + _nextLine, true, true);
    const int pos = _out.mark() - _outBuffer;
    int formattedBytes = 0;
    if (_line->hasValue()) {
        _out.text(" =0x");
        formatHex(_line->value(), 0, true);
        formattedBytes = 0;
    } else {
        formattedBytes = formatBytes(_nextLine);
    }
    const int codeBytes = _line->codeBytes();
    const int dataTextLen =
            _line->config().opCodeWidth() == OPCODE_8BIT ? (codeBytes * 3) : (codeBytes / 2) * 5;
    if (_nextLine == 0)
        formatContent(pos + dataTextLen + 1);
    _nextLine += formattedBytes;
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
