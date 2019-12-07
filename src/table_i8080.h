/* -*- mode: c++; -*- */
#ifndef __TABLE_I8080_H__
#define __TABLE_I8080_H__

#include "config_i8080.h"
#include "insn_i8080.h"

class TableI8080 {
public:
    Error searchName(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;
};

extern TableI8080 TableI8080;
#endif // __TABLE_I8080_H__
