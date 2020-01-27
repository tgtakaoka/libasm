/* -*- mode: c++; -*- */
#ifndef __INSN_Z80_H__
#define __INSN_Z80_H__

#include "insn_base.h"
#include "entry_z80.h"

class Insn : public InsnBase<target::uintptr_t, target::insn_t, 4, 4> {
public:
    AddrMode addrMode() const { return _addrMode(_flags2); }
    InsnFormat insnFormat() const { return _insnFormat(_flags1); }
    OprFormat leftFormat() const { return _oprFormat(_flags1); }
    OprFormat rightFormat() const { return _oprFormat(_flags2); }

    void setFlags(host::uint_t flags1, host::uint_t flags2) {
        _flags1 = flags1;
        _flags2 = flags2;
    }
    void setFlags(const Insn &other) {
        _flags1 = other._flags1;
        _flags2 = other._flags2;
    }

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
    host::uint_t _flags1;
    host::uint_t _flags2;
};

#endif // __INSN_Z80_H__
