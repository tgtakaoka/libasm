/* -*- mode: c++; -*- */
#ifndef __INSN_TMS9995_H__
#define __INSN_TMS9995_H__

#include "insn_base.h"
#include "entry_tms9900.h"

class Insn : public InsnBase<target::uintptr_t, target::insn_t, 6, 4> {
public:
    AddrMode addrMode() const { return _addrMode(_flags); }
    bool is9995() const { return _mcuType(_flags) == TMS9995; }

    void setFlags(host::uint_t flags) { _flags = flags; }

    Error readUint16(DisMemory<target::uintptr_t> &memory, uint16_t &val) {
        uint8_t high, low;
        if (readByte(memory, high)) return NO_MEMORY;
        if (readByte(memory, low))  return NO_MEMORY;
        val = static_cast<uint16_t>(high) << 8 | low;
        return OK;
    }

    void emitInsn() {
        emitUint16(_insnCode, 0);
        if (_insnLen == 0) _insnLen = 2;
    }

    void emitOperand(uint16_t val) {
        if (_insnLen == 0) _insnLen = 2;
        appendUint16(val);
    }

    void appendUint16(uint16_t val) {
        emitUint16(val, _insnLen);
        _insnLen += 2;
    }

private:
    host::uint_t _flags;

    void emitUint16(uint16_t val, host::uint_t pos) {
        _bytes[pos++] = static_cast<uint8_t>(val >> 8);
        _bytes[pos] = static_cast<uint8_t>(val);
    }
};

#endif // __INSN_TMS9900_H__
