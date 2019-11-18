/* -*- mode: c++; -*- */
#ifndef __TABLE_TMS9995_H__
#define __TABLE_TMS9995_H__

#include "config_tms9995.h"
#include "insn_tms9995.h"

class InsnTable {
public:
    Error searchName(Insn &insn) const;
    void searchInsnCode(Insn &insn) const;
};

extern InsnTable InsnTable;
#endif // __TABLE_TMS9995_H__
