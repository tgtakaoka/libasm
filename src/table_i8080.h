/* -*- mode: c++; -*- */
#ifndef __TABLE_I8080_H__
#define __TABLE_I8080_H__

#include "insn_i8080.h"

class TableI8080 {
public:
    Error search(Insn &insn, const char *name) const;
    Error search(Insn &insn, target::insn_t insnCode) const;
};

extern TableI8080 TableI8080;
#endif // __TABLE_I8080_H__
