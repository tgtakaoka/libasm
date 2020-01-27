/* -*- mode: c++; -*- */
#ifndef __INSN_I8080_H__
#define __INSN_I8080_H__

#include "insn_base.h"
#include "entry_i8080.h"

class Insn : public InsnBase<target::uintptr_t, target::insn_t, 3, 4> {
public:
    AddrMode addrMode() const { return _addrMode(_flags); }
    InsnFormat insnFormat() const { return _insnFormat(_flags); }

    void setFlags(host::uint_t flags) { _flags = flags; }

    Error readUint16(DisMemory<target::uintptr_t> &memory, uint16_t &val) {
        uint8_t low, high;
        if (readByte(memory, low))  return NO_MEMORY;
        if (readByte(memory, high)) return NO_MEMORY;
        val = static_cast<uint16_t>(high) << 8 | low;
        return OK;
    }

    void emitUint16(uint16_t val) {
        emitByte(static_cast<uint8_t>(val & 0xff));
        emitByte(static_cast<uint8_t>(val >> 8));
    }

private:
    host::uint_t _flags;
};

#endif // __INSN_I8080_H__
