/* -*- mode: c++; -*- */
#ifndef __TABLE_R65C02_H__
#define __TABLE_R65C02_H__

#include "config_r65c02.h"
#include "insn_r65c02.h"

class InsnTable {
public:
    Error searchName(Insn &insn) const;
    Error searchNameAndAddrMode(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;
};

extern InsnTable InsnTable;

#endif // __TABLE_R65C02_H__
