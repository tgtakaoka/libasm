/* -*- mode: c++; -*- */
#ifndef __INSN_MC6809_H__
#define __INSN_MC6809_H__

#include "insn_base.h"
#include "entry_mc6809.h"

class Insn : public InsnBase<ENDIAN_BIG, 5, 6> {
public:
    AddrMode addrMode() const { return _addrMode(_flags); }
    OprSize oprSize() const { return _oprSize(_flags); }
    bool is6309() const { return _mcuType(_flags) == HD6309; }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = ::_flags(
            _mcuType(_flags), _oprSize(_flags), _addrMode(addrMode));
    }

private:
    host::uint_t _flags;
};

#endif // __INSN_MC6809_H__
