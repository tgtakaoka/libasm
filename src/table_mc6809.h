/* -*- mode: c++; -*- */
#ifndef __TABLE_MC6809_H__
#define __TABLE_MC6809_H__

#include "config_mc6809.h"
#include "insn_mc6809.h"

#define PSEUDO_SETDP   0x00     // reuse NEG opecode
#define PSEUDO_ASSUME  0x03     // reuse COM opecode

struct EntryPage {
    const target::opcode_t prefix;
    const Entry *const table;
    const Entry *const end;
};

class TableMc6809 {
public:
    Error searchName(Insn &insn) const;
    Error searchNameAndAddrMode(Insn &insn) const;
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

private:
    static const Entry *searchEntry(
        const char *name, const Entry *table, const Entry *end);
    static const Entry *searchEntry(
        const target::opcode_t opCode, const Entry *table, const Entry *end);

    static Error searchPages(
        Insn &insn, const char *name, const EntryPage *pages, const EntryPage *end);
    static Error searchPages(
        Insn &insn, const char *name, AddrMode addrMode,
        const EntryPage *pages, const EntryPage *end);
    static Error searchPages(
        Insn &insn, target::insn_t insnCode, const EntryPage *pages, const EntryPage *end);
};

extern TableMc6809 TableMc6809;

#endif // __TABLE_MC6809_H__
