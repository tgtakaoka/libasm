/* -*- mode: c++; -*- */
#ifndef __INSN_TABLE_H__
#define __INSN_TABLE_H__

#include "config_host.h"

class InsnTable {
protected:
    static void setInsnCode(Insn &insn, target::insn_t insnCode) { insn.setInsnCode(insnCode); }
    static void setOprLen(Insn &insn, host::uint_t oprLen) { insn.setOprLen(oprLen); }
    static void setAddrMode(Insn &insn, AddrMode addrMode) { insn.setAddrMode(addrMode); }
    static void setName(Insn &insn, const char *name) { insn.setName(name); }
};

#endif // __INSN_TABLE_H__
