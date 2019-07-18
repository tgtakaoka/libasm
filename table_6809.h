/* -*- mode: c++; -*- */
#ifndef __TABLE_6809_H__
#define __TABLE_6809_H__

#include "insn_table.h"

class Table6809 : public InsnTable {
public:
    bool isPrefixCode(target::opcode_t opCode) const override;
    Error search(Insn &insn, const char *name) const override;
    Error search(Insn &insn, AddrMode mode) const;
};

#endif // __TABLE_6809_H__
