/* -*- mode: c++; -*- */
#ifndef __TABLE_HD6309_H__
#define __TABLE_HD6309_H__

#include "table_mc6809.h"

class TableHd6309 : public TableMc6809 {
public:
    virtual Error search(Insn &insn, const char *name) const override;
    virtual Error search(Insn &insn, AddrMode mode) const override;
    virtual Error search(Insn &insn, target::insn_t insnCode) const override;
};

extern TableHd6309 TableHd6309;
#endif // __TABLE_HD6309_H__
