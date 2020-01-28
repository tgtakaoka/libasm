/* -*- mode: c++; -*- */
#ifndef __REG_M6502_H__
#define __REG_M6502_H__

enum RegName : char {
    REG_UNDEF = 0,
    REG_A = 'A',
    REG_X = 'X',
    REG_Y = 'Y',
};

#include "reg_base.h"

class RegM6502 : public RegBase {
public:
    host::uint_t regNameLen(RegName regName) const;
    bool compareRegName(const char *line, RegName regName) const;
    RegName parseIndexReg(const char *line) const;
    char *outRegName(char *out, const RegName regName) const;
};

#endif // __REG_M6502_H__
