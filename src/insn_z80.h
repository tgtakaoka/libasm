/* -*- mode: c++; -*- */
#ifndef __INSN_Z80_H__
#define __INSN_Z80_H__

#include "insn_base.h"
#include "entry_z80.h"

class Insn : public InsnBase<uint8_t, ENDIAN_LITTLE, 4, 4> {
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

private:
    host::uint_t _flags1;
    host::uint_t _flags2;
};

#endif // __INSN_Z80_H__
