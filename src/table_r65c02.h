/* -*- mode: c++; -*- */
#ifndef __TABLE_R65C02_H__
#define __TABLE_R65C02_H__

#include "config_r65c02.h"
#include "insn_r65c02.h"

class TableR65c02Base {
protected:
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

template<McuType mcuType = R65C02>
class TableR65c02 : public TableR65c02Base {
public:
    static const TableR65c02<mcuType> *table();
    
    Error searchName(Insn &insn) const;
    Error searchNameAndAddrMode(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;
};

#endif // __TABLE_R65C02_H__
