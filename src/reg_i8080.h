/* -*- mode: c++; -*- */
#ifndef __REGISTER_I8080_H__
#define __REGISTER_I8080_H__

enum RegName : char {
    REG_UNDEF = 0,
    REG_B = 'B',
    REG_C = 'C',
    REG_D = 'D',
    REG_E = 'E',
    REG_H = 'H',
    REG_L = 'L',
    REG_M = 'M',
    REG_A = 'A',
    REG_SP = 'S',
    REG_PSW = 'P',
};

#include "reg_base.h"

class RegI8080 : public RegBase {
public:
    RegName parseRegister(const char *line) const;
    RegName parsePointerReg(const char *line) const;
    RegName parseStackReg(const char *line) const;
    RegName parseIndexReg(const char *line) const;
    RegName parseDataReg(const char *line) const;

    static host::int_t encodePointerReg(RegName regName);
    static host::int_t encodeStackReg(RegName regName);
    static host::int_t encodeIndexReg(RegName regName);
    static host::int_t encodeDataReg(RegName regName);

    static RegName decodePointerReg(uint8_t regNum);
    static RegName decodeStackReg(uint8_t regNum);
    static RegName decodeIndexReg(uint8_t regNum);
    static RegName decodeDataReg(uint8_t regNum);

    bool compareRegName(const char *line, RegName regName) const;
    host::uint_t regNameLen(RegName regName) const;

    char *outRegName(char *out, const RegName regName) const;

private:
    RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end) const;
    char regName1stChar(RegName regName) const;
    char regName2ndChar(RegName regName) const;
    char regName3rdChar(RegName regName) const;
};

#endif // __REGISTER_I8080_H__
