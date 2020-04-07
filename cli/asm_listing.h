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

#ifndef __ASM_LISTING_H__
#define __ASM_LISTING_H__

#include <string>

namespace libasm {
namespace cli {

template<typename Address>
class AsmLine {
public:
    virtual uint16_t lineNumber() const = 0;
    virtual uint16_t includeNest() const = 0;
    virtual Address startAddress() const = 0;
    virtual int generatedSize() const = 0;
    virtual uint8_t getByte(int offset) const = 0;
    virtual bool hasValue() const = 0;
    virtual uint32_t value() const = 0;
    virtual bool hasLabel() const = 0;
    virtual bool hasInstruction() const = 0;
    virtual bool hasOperand() const = 0;
    virtual bool hasComment() const = 0;
    virtual std::string getLabel() const = 0;
    virtual std::string getInstruction() const = 0;
    virtual std::string getOperand() const = 0;
    virtual std::string getComment() const = 0;
    virtual int maxBytes() const = 0;
    virtual int labelWidth() const = 0;
    virtual int instructionWidth() const = 0;
    virtual int operandWidth() const = 0;
};

template<typename Address>
class AsmListing {
public:
    void reset(
        AsmLine<Address> &line,
        size_t insnUnitSize,
        bool uppercase,
        bool lineNumner) {
        _line = &line;
        _insnUnitSize = insnUnitSize;
        _uppercase = uppercase;
        _lineNumber = lineNumner;
        _next = 0;
    }

    bool hasNext() const {
        return _next < _line->generatedSize();
    }

    const char *getContent() {
        _out.clear();
        formatContent(0);
        _next = _line->generatedSize();
        return _out.c_str();
    }

    const char *getLine() {
        _out.clear();
        formatLine();
        return _out.c_str();
    }

private:
    const AsmLine<Address> *_line;
    size_t _insnUnitSize;
    bool _uppercase = false;
    bool _lineNumber = false;
    int _next;
    std::string _out;

    void formatHex(uint8_t val) {
        val &= 0xf;
        if (val < 10) {
            _out += '0' + val;
        } else if (_uppercase) {
            _out += 'A' + val - 10;
        } else {
            _out += 'a' + val - 10;
        }
    }
    void formatUint8(
        uint8_t val, bool fixedWidth = true, bool zeroSuppress = false) {
        const uint8_t msd = (val >> 4);
        if (msd || !zeroSuppress) {
            formatHex(msd);
        } else if (fixedWidth) {
            _out += ' ';
        }
        formatHex(val);
    }
    void formatUint16(
        uint16_t val, bool fixedWidth = true, bool zeroSuppress = false) {
        const uint8_t msb = static_cast<uint8_t>(val >> 8);
        if (msb || !zeroSuppress) {
            formatUint8(msb, fixedWidth, zeroSuppress);
            zeroSuppress = false;
        } else if (fixedWidth) {
            _out += "  ";
        }
        formatUint8(static_cast<uint8_t>(val), fixedWidth, zeroSuppress);
    }
    void formatUint32(
        uint32_t val, bool fixedWidth = true, bool zeroSuppress = false) {
        const uint16_t msw = static_cast<uint16_t>(val >> 16);
        if (msw || !zeroSuppress) {
            formatUint16(msw, fixedWidth, zeroSuppress);
            zeroSuppress = false;
        } else if (fixedWidth) {
            _out += "    ";
        }
        formatUint16(static_cast<uint16_t>(val), fixedWidth, zeroSuppress);
    }
    void formatAddress(
        Address addr, bool fixedWidth = true, bool zeroSuppress = false) {
        if (sizeof(Address) == 2) {
            formatUint16(addr, fixedWidth, zeroSuppress);
        } else {
            formatUint32(addr, fixedWidth, zeroSuppress);
        }
        _out += ':';
    }
    int formatBytes(int base) {
        const int maxBytes = _line->maxBytes();
        int i = 0;
        while (base + i < _line->generatedSize() && i < maxBytes) {
            uint8_t val = _line->getByte(base + i);
            if (_insnUnitSize == 1 || (i % 2) == 0)
                _out += ' ';
            formatUint8(val);
            i++;
        }
        return i;
    }
    void formatTab(size_t pos, int delta = 4) {
        while (pos < _out.size() + 1)
            pos += delta;
        while (_out.size() < pos)
            _out += ' ';
    }

    void formatContent(int pos) {
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
            _out += _line->getInstruction();
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

    void formatLine() {
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
        const int dataTextLen = _insnUnitSize == 2
            ? (_line->maxBytes() / 2) * 5
            : (_line->maxBytes() * 3);
        if (_next == 0) formatContent(pos + dataTextLen + 1);
        _next += formattedBytes;
    }
};

} // namespace cli
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
