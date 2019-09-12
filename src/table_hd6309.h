/* -*- mode: c++; -*- */
#ifndef __TABLE_HD6309_H__
#define __TABLE_HD6309_H__

#include "config_hd6309.h"
#include "insn_hd6309.h"

struct EntryPage {
    const target::opcode_t prefix;
    const Entry *const table;
    const Entry *const end;
};

class InsnTableUtils {
public:
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

protected:
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

template<McuType mcuType = HD6309>
class InsnTable : public InsnTableUtils {
public:
    static const InsnTable<mcuType> *table();

    Error searchName(Insn &insn) const;
    Error searchNameAndAddrMode(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;
};

#endif // __TABLE_HD6309_H__
