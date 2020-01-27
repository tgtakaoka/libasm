/* -*- mode: c++; -*- */
#ifndef __INSN_MC6809_H__
#define __INSN_MC6809_H__

#include "insn_base.h"
#include "entry_mc6809.h"

class Insn : public InsnBase<target::uintptr_t, target::insn_t, 5, 6> {
public:
    AddrMode addrMode() const { return _addrMode(_flags); }
    OprSize oprSize() const { return _oprSize(_flags); }
    bool is6309() const { return _mcuType(_flags) == HD6309; }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = ::_flags(_mcuType(_flags), _oprSize(_flags), _addrMode(addrMode));
    }

    Error readUint16(DisMemory<target::uintptr_t> &memory, uint16_t &val) {
        uint8_t high, low;
        if (readByte(memory, high)) return NO_MEMORY;
        if (readByte(memory, low))  return NO_MEMORY;
        val = static_cast<uint16_t>(high) << 8 | low;
        return OK;
    }

    Error readUint32(DisMemory<target::uintptr_t> &memory, uint32_t &val) {
        uint16_t high, low;
        if (readUint16(memory, high)) return NO_MEMORY;
        if (readUint16(memory, low))  return NO_MEMORY;
        val = static_cast<uint32_t>(high) << 16 | low;
        return OK;
    }

    void emitUint16(uint16_t val) {
        emitByte(static_cast<uint8_t>(val >> 8));
        emitByte(static_cast<uint8_t>(val & 0xff));
    }

    void emitUint32(uint32_t val) {
        emitUint16(static_cast<uint16_t>(val >> 16));
        emitUint16(static_cast<uint16_t>(val & 0xffff));
    }

private:
    host::uint_t _flags;
};

#endif // __INSN_MC6809_H__
