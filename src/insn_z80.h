/* -*- mode: c++; -*- */
#ifndef __INSN_Z80_H__
#define __INSN_Z80_H__

#include "insn_base.h"
#include "entry_z80.h"

class Insn
    : public InsnBase<ENDIAN_LITTLE, Entry::code_max, Entry::name_max> {
public:
    AddrMode addrMode() const { return Entry::_addrMode(_flags2); }
    InsnFormat insnFormat() const { return Entry::_insnFormat(_flags1); }
    OprFormat leftFormat() const { return Entry::_oprFormat(_flags1); }
    OprFormat rightFormat() const { return Entry::_oprFormat(_flags2); }

    void setFlags(host::uint_t flags1, host::uint_t flags2) {
        _flags1 = flags1;
        _flags2 = flags2;
    }
    void setFlags(const Insn &other) {
        _flags1 = other._flags1;
        _flags2 = other._flags2;
    }

    void embed(target::opcode_t data) {
        _insnCode |= data;
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
    host::uint_t _flags1;
    host::uint_t _flags2;
};

#endif // __INSN_Z80_H__
