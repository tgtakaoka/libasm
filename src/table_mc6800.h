/* -*- mode: c++; -*- */
#ifndef __TABLE_MC6800_H__
#define __TABLE_MC6800_H__

#include "config_mc6800.h"
#include "insn_mc6800.h"

class TableMc6800 {
public:
    Error searchName(Insn &insn) const;
    Error searchNameAndAddrMode(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;

    bool setCpu(const char *cpu);
    static const char *listCpu();

private:
    static const Entry *searchEntry(
        const char *name, const Entry *table, const Entry *end);
    static const Entry *searchEntry(
        const target::opcode_t opCode, const Entry *table, const Entry *end);
};

extern TableMc6800 TableMc6800;

#endif // __TABLE_MC6800_H__
