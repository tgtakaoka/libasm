/* -*- mode: c++; -*- */
#ifndef __ASM_H__
#define __ASM_H__

#include "insn.h"

class Asm {
protected:
    static void resetAddress(Insn &insn, target::uintptr_t addr) { insn.reset(addr); }
    static void setName(Insn &insn, const char *name, const char *end) { insn.setName(name, end); }
    static void setInsnCode(Insn &insn, target::insn_t insnCode) { insn.setInsnCode(insnCode); }
    static void setAddrMode(Insn &insn, AddrMode addrMode) { insn.setAddrMode(addrMode); }
    static void emitByte(Insn &insn, target::byte_t val) { insn.addByte(val); }
    static void emitUint16(Insn &insn, target::uint16_t val) { insn.addUint16(val); }
    static void emitUint32(Insn &insn, target::uint32_t val) { insn.addUint32(val); }
};

#endif // __ASM_H__
