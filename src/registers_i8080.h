/* -*- mode: c++; -*- */
#ifndef __REGISTER_I8080_H__
#define __REGISTER_I8080_H__

#include "config_i8080.h"

enum RegName : char {
    NONE = 0,
    B = 'B',
    C = 'C',
    D = 'D',
    E = 'E',
    H = 'H',
    L = 'L',
    M = 'M',
    A = 'A',
    SP = 'S',
    PSW = 'P',
};

class Registers {
public:
    static RegName parsePointerReg(const char *line);
    static RegName parseStackReg(const char *line);
    static RegName parseIndexReg(const char *line);
    static RegName parseDataReg(const char *line);

    static host::int_t encodePointerReg(RegName regName);
    static host::int_t encodeStackReg(RegName regName);
    static host::int_t encodeIndexReg(RegName regName);
    static host::int_t encodeDataReg(RegName regName);

    static RegName decodePointerReg(target::byte_t regNum);
    static RegName decodeStackReg(target::byte_t regNum);
    static RegName decodeIndexReg(target::byte_t regNum);
    static RegName decodeDataReg(target::byte_t regNum);

    static bool compareRegName(const char *line, RegName regName);
    static host::uint_t regNameLen(RegName regName);

    static char *outRegName(char *out, const RegName regName);
    static char *outCCRBits(char *out, target::byte_t val);

private:
    static RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end);
};

#endif // __REGISTER_I8080_H__
