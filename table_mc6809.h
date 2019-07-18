/* -*- mode: c++; -*- */
#ifndef __TABLE_MC6809_H__
#define __TABLE_MC6809_H__

#include "insn_table.h"

class TableMc6809 : public InsnTable {
public:
    Error search(Insn &insn, const char *name) const;
    Error search(Insn &insn, AddrMode mode) const;
};

#endif // __TABLE_MC6809_H__
