/* -*- mode: c++; -*- */
#ifndef __INSN_MC6800_H__
#define __INSN_MC6800_H__

#include "insn_base.h"
#include "entry_mc6800.h"

class Insn
    : public InsnBase<ENDIAN_BIG, Entry::code_max, Entry::name_max> {
public:
    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    InsnAdjust insnAdjust() const { return Entry::_insnAdjust(_flags); }
    OprSize oprSize() const { return Entry::_oprSize(_flags); }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = Entry::_flags(
            addrMode, Entry::_insnAdjust(_flags), Entry::_oprSize(_flags));
    }

    void embed(target::opcode_t data) {
        _insnCode |= data;
    }
    void emitInsn() {
        emitByte(_insnCode);
    }

private:
    host::uint_t _flags;
};

#endif // __INSN_MC6800_H__
