/* -*- mode: c++; -*- */
#ifndef __TABLE_MC68000_H__
#define __TABLE_MC68000_H__

#include "config_mc68000.h"
#include "insn_mc68000.h"

class TableMc68000 {
public:
    Error searchName(Insn &insn) const;
    Error searchNameAndAddrMode(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;
};

extern TableMc68000 TableMc68000;

#endif // __TABLE_MC68000_H__
