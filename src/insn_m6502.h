/* -*- mode: c++; -*- */
#ifndef __INSN_M6502_H__
#define __INSN_M6502_H__

#include "insn_base.h"
#include "entry_m6502.h"

class Insn : public InsnBase<ENDIAN_LITTLE, 3, 4> {
public:
    AddrMode addrMode() const { return _addrMode(_flags); }
    bool is65c02() const { return _mcuType(_flags) == W65C02; }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = ::_flags(_mcuType(_flags), _addrMode(addrMode));
    }

private:
    host::uint_t _flags;
};

#endif // __INSN_M6502_H__
