/* -*- mode: c++; -*- */
#ifndef __TABLE_Z80_H__
#define __TABLE_Z80_H__

#include "config_z80.h"
#include "insn_z80.h"

class InsnTable {
public:
    Error searchNameAndOprFormats(
        Insn &insn, OprFormat leftOpr, OprFormat rightOpr) const;
    Error searchInsnCode(Insn &insn) const;

    static target::insn_t insnCode(
        target::opcode_t prefixCode, target::opcode_t opCode) {
        return (target::insn_t(prefixCode) << 8 | opCode);
    }
    static target::opcode_t prefixCode(target::insn_t insnCode) {
        return target::opcode_t(insnCode >> 8);
    }
    static target::opcode_t opCode(target::insn_t insnCode) {
        return target::opcode_t(insnCode & 0xff);
    }
    static bool isPrefixCode(target::opcode_t opCode);

    static RegName decodeIndexReg(target::insn_t insnCode);
};

extern InsnTable InsnTable;

#endif // __TABLE_Z80_H__
