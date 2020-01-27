/* -*- mode: c++; -*- */
#ifndef __INSN_M6502_H__
#define __INSN_M6502_H__

#include "insn_base.h"
#include "entry_m6502.h"

class Insn : public InsnBase<target::uintptr_t, target::insn_t, 3, 4>{
public:
    AddrMode addrMode() const { return _addrMode(_flags); }
    bool is65c02() const { return _mcuType(_flags) == W65C02; }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = ::_flags(_mcuType(_flags), _addrMode(addrMode));
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
    host::uint_t _flags;
};

#endif // __INSN_M6502_H__
