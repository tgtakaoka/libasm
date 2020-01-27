/* -*- mode: c++; -*- */
#ifndef __ASM_LISTING_H__
#define __ASM_LISTING_H__

#include "cli_memory.h"

#include <string>

template<typename Address>
class AsmLine {
public:
    virtual Address startAddress() const = 0;
    virtual int generatedSize() const = 0;
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
        CliMemory<Address> &memory,
        bool wording,
        bool uppercase) {
        _line = &line;
        _memory = &memory;
        _wording = wording;
        _uppercase = uppercase;
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
    const CliMemory<Address> *_memory;
    bool _wording = false;      // 16 bit output if true
    bool _uppercase = false;
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
    void formatUint8(uint8_t val) {
        formatHex(val >> 4);
        formatHex(val);
    }
    void formatUint16(uint16_t val) {
        formatUint8(static_cast<uint8_t>(val >> 8));
        formatUint8(static_cast<uint8_t>(val));
    }
    void formatUint32(uint32_t val) {
        formatUint16(static_cast<uint16_t>(val >> 16));
        formatUint16(static_cast<uint16_t>(val));
    }
    void formatAddress(Address addr) {
        if (sizeof(Address) == 2) {
            formatUint16(addr);
        } else {
            formatUint32(addr);
        }
        _out += ':';
    }
    int formatBytes(Address addr, int base) {
        const int maxBytes = _line->maxBytes();
        int i = 0;
        while (base + i < _line->generatedSize() && i < maxBytes) {
            uint8_t val = 0;
            _memory->readByte(addr + base+ i, val);
            if (!_wording || (i % 2) == 0)
                _out += ' ';
            formatUint8(val);
            i++;
        }
        return i;
    }
    void formatTab(int pos) {
        while (static_cast<int>(_out.size()) < pos)
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
            _out += ' ';
            formatTab(pos + _line->labelWidth());
            _out += _line->getInstruction();
        }
        if (_line->hasOperand()) {
            _out += ' ';
            formatTab(pos + _line->labelWidth()
                      + _line->instructionWidth());
            _out += _line->getOperand();
        }
        if (_line->hasComment()) {
            _out += ' ';
            formatTab(pos + _line->labelWidth()
                      + _line->instructionWidth() + _line->operandWidth());
            _out += _line->getComment();
        }
    }

    void formatLine() {
        formatAddress(_line->startAddress() + _next);
        const int pos = _out.size();
        const int n = formatBytes(_line->startAddress(), _next);
        const int dataTextLen = _wording
            ? (_line->maxBytes() / 2) * 5
            : (_line->maxBytes() * 3);
        if (_next == 0) formatContent(pos + dataTextLen + 1);
        _next += n;
    }
};

#endif
