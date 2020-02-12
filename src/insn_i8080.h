/* -*- mode: c++; -*- */
#ifndef __INSN_I8080_H__
#define __INSN_I8080_H__

#include "insn_base.h"
#include "entry_i8080.h"

class Insn : public InsnBase<ENDIAN_LITTLE, 3, 4> {
public:
    AddrMode addrMode() const { return _addrMode(_flags); }
    InsnFormat insnFormat() const { return _insnFormat(_flags); }

    void setFlags(host::uint_t flags) { _flags = flags; }

private:
    host::uint_t _flags;
};

#endif // __INSN_I8080_H__
