/* -*- mode: c++; -*- */
#ifndef __INSN_MC6809_H__
#define __INSN_MC6809_H__

#include "insn_base.h"
#include "entry_mc6809.h"

class Insn
    : public InsnBase<ENDIAN_BIG, Entry::code_max, Entry::name_max> {
public:
    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    OprSize oprSize() const { return Entry::_oprSize(_flags); }
    bool is6309() const { return Entry::_cpuType(_flags) == HD6309; }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = Entry::_flags(
            Entry::_cpuType(_flags),
            Entry::_oprSize(_flags),
            addrMode);
    }

    void setInsnCode(target::opcode_t prefixCode, target::opcode_t opCode) {
        _insnCode = (static_cast<target::insn_t>(prefixCode) << 8) | opCode;
    }
    bool hasPrefix() const { return prefixCode() != 0; }
    target::opcode_t prefixCode() const {
        return static_cast<target::opcode_t>(_insnCode >> 8);
    }
    target::opcode_t opCode() const {
        return static_cast<target::opcode_t>(_insnCode);
    }

    void emitInsn() {
        if (hasPrefix())
            emitByte(prefixCode());
        emitByte(opCode());
    }

private:
    host::uint_t _flags;
};

#endif // __INSN_MC6809_H__
