/* -*- mode: c++; -*- */
#ifndef __TABLE_I8080_H__
#define __TABLE_I8080_H__

#include "config_i8080.h"

class InsnTable {
public:
    Error searchName(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;
};

extern InsnTable InsnTable;
#endif // __TABLE_I8080_H__
