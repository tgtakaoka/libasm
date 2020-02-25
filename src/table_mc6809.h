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

    bool setCpu(const char *cpu);
    static const char *listCpu();
    bool is6309() const { return _cpuType == HD6309; }

    static bool isPrefixCode(target::opcode_t opCode);

private:
    CpuType _cpuType;

    static const Entry *searchEntry(
        const char *name, const Entry *table, const Entry *end);
    static const Entry *searchEntry(
        const target::opcode_t opCode, const Entry *table, const Entry *end);

    static Error searchName(
        Insn &insn, const EntryPage *pages, const EntryPage *end);
    static Error searchNameAndAddrMode(
        Insn &insn, const EntryPage *pages, const EntryPage *end);
    static Error searchInsnCode(
        Insn &insn, const EntryPage *pages, const EntryPage *end);
};

extern TableMc6809 TableMc6809;

#endif // __TABLE_MC6809_H__
