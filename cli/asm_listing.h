/* -*- mode: c++; -*- */
#ifndef __ASM_LISTING_H__
#define __ASM_LISTING_H__

#include "cli_memory.h"

#include <string>

template <typename Addr>
class AsmLine {
public:
    virtual Addr startAddress() const = 0;
    virtual int generatedSize() const = 0;
    virtual bool hasLabel() const = 0;
    virtual bool hasInstruction() const = 0;
    virtual bool hasOperand() const = 0;
    virtual bool hasComment() const = 0;
    virtual std::string getLabel() const = 0;
    virtual std::string getInstruction() const = 0;
    virtual std::string getOperand() const = 0;
    virtual std::string getComment() const = 0;
};

template <typename Addr, bool wordBase>
class AsmListing {
public:
    AsmListing(
        AsmLine<Addr> &line,
        CliMemory<Addr> &memory,
        bool uppercase = false)
        : _line(line),
          _memory(memory),
          _addr(line.startAddress()),
          _next(0),
          _uppercase(uppercase)
    {}

    bool hasNext() const {
        return _next < _line.generatedSize();
    }

    const char *getNext() {
        _out.clear();
        formatNext();
        return _out.c_str();
    }

private:
    const AsmLine<Addr> &_line;
    const CliMemory<Addr> &_memory;
    Addr _addr;
    int _next;
    bool _uppercase;
    std::string _out;

    void formatChar(char c) {
        _out = _out + c;
    }
    void formatString(const std::string &str) {
        _out.append(str);
    }
    void formatHex(uint8_t val) {
        val &= 0xf;
        if (val < 10) {
            formatChar('0' + val);
        } else if (_uppercase) {
            formatChar('A' + val - 10);
        } else {
            formatChar('a' + val - 10);
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
    void formatTab(int pos) {
        while (static_cast<int>(_out.size()) < pos)
            formatChar(' ');
    }

    void formatAddress(Addr addr) {
        if (sizeof(Addr) == 2) {
            formatUint16(addr);
        } else {
            formatUint32(addr);
        }
        formatChar(':');
    }

    int formatBytes(Addr addr, int base) {
        int i = 0;
        while (base + i < _line.generatedSize() && i < 6) {
            uint8_t val = 0;
            _memory.readByte(addr + base+ i, val);
            if (!wordBase || (i % 2) == 0)
                formatChar(' ');
            formatUint8(val);
            i++;
        }
        return i;
    }

    void formatNext() {
        formatAddress(_addr + _next);
        int pos = _out.size();
        const int n = formatBytes(_addr, _next);
        if (_next == 0) {
            if (_line.hasLabel()) {
                formatTab(pos + 18);
                formatString(_line.getLabel());
            }
            if (!_line.hasInstruction() && _line.hasComment()) {
                formatTab(pos + 18);
                formatString(_line.getComment());
                return;
            }
            if (_line.hasInstruction()) {
                formatChar(' ');
                formatTab(pos + 26);
                formatString(_line.getInstruction());
            }
            if (_line.hasOperand()) {
                formatChar(' ');
                formatTab(pos + 32);
                formatString(_line.getOperand());
            }
            if (_line.hasComment()) {
                formatChar(' ');
                formatTab(pos + 40);
                formatString(_line.getComment());
            }
        }
        _next += n;
    }
};

#endif
