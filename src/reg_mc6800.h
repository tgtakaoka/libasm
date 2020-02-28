/* -*- mode: c++; -*- */
#ifndef __REG_MC6800_H__
#define __REG_MC6800_H__

enum RegName : char {
    REG_UNDEF = 0,
    REG_A = 'A',
    REG_B = 'B',
    REG_X = 'X',
};

#include "reg_base.h"

class RegMc6800 : public RegBase {
public:
    RegName parseRegName(const char *line) const;

    char *outRegName(char *out, const RegName regName) const;

    bool compareRegName(const char *line, RegName regName) const;
    host::uint_t regNameLen(RegName regName) const;

private:
    char regName1stChar(const RegName regName) const;
};

#endif // __REG_MC6800_H__
