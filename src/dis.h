/* -*- mode: c++; -*- */
#ifndef __DIS_H__
#define __DIS_H__

#include "insn.h"

class Dis {
protected:
    static void resetAddress(Insn &insn, target::uintptr_t addr) { insn.reset(addr); }
    static void setAddrMode(Insn &insn, AddrMode addrMode) { insn.setAddrMode(addrMode); }
    static void setInsnCode(Insn &insn, target::insn_t insnCode) { insn.setInsnCode(insnCode); }
    static void emitByte(Insn &insn, target::byte_t val) { insn.addByte(val); }
    static void emitUint16(Insn &insn, target::uint16_t val) { insn.addUint16(val); }
    static void emitUint32(Insn &insn, target::uint32_t val) { insn.addUint32(val); }
};

#endif // __DIS_H__
