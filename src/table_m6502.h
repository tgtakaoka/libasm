/* -*- mode: c++; -*- */
#ifndef __TABLE_M6502_H__
#define __TABLE_M6502_H__

#include "config_m6502.h"
#include "insn_m6502.h"

class TableM6502 {
public:
    Error searchName(Insn &insn) const;
    Error searchNameAndAddrMode(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;

    void setMcuType(McuType mcuType) { _mcuType = mcuType; }
    bool is65c02() const { return _mcuType == W65C02; }

private:
    McuType _mcuType;

    static const Entry *searchEntry(
        const char *name, const Entry *table, const Entry *end);
    static const Entry *searchEntry(
        const target::insn_t insnCode, const Entry *table, const Entry *end);

    static Error searchPages(
        Insn &insn, const char *name, const Entry *table, const Entry *end);
    static Error searchPages(
        Insn &insn, const char *name, AddrMode addrMode,
        const Entry *table, const Entry *end);
    static Error searchPages(
        Insn &insn, target::insn_t insnCode, const Entry *table, const Entry *end);
};

extern TableM6502 TableM6502;

#endif // __TABLE_M6502_H__
