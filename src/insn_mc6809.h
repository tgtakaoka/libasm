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
