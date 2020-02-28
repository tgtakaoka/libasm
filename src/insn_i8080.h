/* -*- mode: c++; -*- */
#ifndef __INSN_I8080_H__
#define __INSN_I8080_H__

#include "insn_base.h"
#include "entry_i8080.h"

class Insn
    : public InsnBase<ENDIAN_LITTLE, Entry::code_max, Entry::name_max> {
public:
    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    InsnFormat insnFormat() const { return Entry::_insnFormat(_flags); }

    void setFlags(host::uint_t flags) { _flags = flags; }

    void embed(target::opcode_t data) {
        _insnCode |= data;
    }

private:
    host::uint_t _flags;
};

#endif // __INSN_I8080_H__
