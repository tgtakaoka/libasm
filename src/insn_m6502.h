/* -*- mode: c++; -*- */
#ifndef __INSN_M6502_H__
#define __INSN_M6502_H__

#include "insn_base.h"
#include "entry_m6502.h"

class Insn
    : public InsnBase<ENDIAN_LITTLE, Entry::code_max, Entry::name_max> {
public:
    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    bool is65c02() const { return Entry::_cpuType(_flags) == W65C02; }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = Entry::_flags(Entry::_cpuType(_flags), addrMode);
    }

    void emitInsn() {
        emitByte(_insnCode);
    }

private:
    host::uint_t _flags;
};

#endif // __INSN_M6502_H__
