/* -*- mode: c++; -*- */
#ifndef __REG_TMS9900_H__
#define __REG_TMS9900_H__

enum RegName : char {
    REG_UNDEF = 0,
    REG_R0 = '0',
    REG_R1 = '1',
    REG_R2 = '2',
    REG_R3 = '3',
    REG_R4 = '4',
    REG_R5 = '5',
    REG_R6 = '6',
    REG_R7 = '7',
    REG_R8 = '8',
    REG_R9 = '9',
    REG_R10 = 'A',
    REG_R11 = 'B',
    REG_R12 = 'C',
    REG_R13 = 'D',
    REG_R14 = 'E',
    REG_R15 = 'F',
};

#include "reg_base.h"

class RegTms9900 : public RegBase {
public:
    host::uint_t regNameLen(RegName regName) const;
    RegName parseRegName(const char *line) const;
    char *outRegName(char *out, host::uint_t regno) const;
    uint16_t encodeRegNumber(RegName regName) const;
};

#endif // __REG_TMS9900_H__
