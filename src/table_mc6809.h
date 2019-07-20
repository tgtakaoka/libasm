/* -*- mode: c++; -*- */
#ifndef __TABLE_MC6809_H__
#define __TABLE_MC6809_H__

#include "config_mc6809.h"
#include "entry_mc6809.h"

class TableMc6809 {
public:
    virtual Error search(Insn &insn, const char *name) const;
    virtual Error search(Insn &insn, AddrMode mode) const;
    virtual Error search(Insn &insn, target::insn_t insnCode) const;

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

    struct EntryPage {
        const target::opcode_t prefix;
        const EntryMc6809 *const table;
        const EntryMc6809 *const end;
    };

protected:
    static const EntryMc6809 *searchEntry(
        const char *name, const EntryMc6809 *table, const EntryMc6809 *end);
    static const EntryMc6809 *searchEntry(
        const char *name, AddrMode mode,
        const EntryMc6809 *table, const EntryMc6809 *end);
    static const EntryMc6809 *searchEntry(
        target::opcode_t opCode,
        const EntryMc6809 *table, const EntryMc6809 *end);

    Error searchPages(Insn &insn, const char *name,
                      const EntryPage *pages, const EntryPage *end) const;
    Error searchPages(Insn &insn, AddrMode mode,
                      const EntryPage *pages, const EntryPage *end) const;
    Error searchPages(Insn &insn, target::insn_t insnCode,
                      const EntryPage *pages, const EntryPage *end) const;
};

extern TableMc6809 TableMc6809;

#endif // __TABLE_MC6809_H__
