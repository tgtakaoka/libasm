/* -*- mode: c++; -*- */
#ifndef __REGISTER_Z80_H__
#define __REGISTER_Z80_H__

#include "config_z80.h"

enum RegName : char {
    NONE = 0,
    HL = 'h',
    BC = 'b',
    DE = 'd',
    SP = 's',
    AF = 'a',
    IX = 'x',
    IY = 'y',
    B = 'B',
    C = 'C',
    D = 'D',
    E = 'E',
    H = 'H',
    L = 'L',
    M = 'M',                            // (HL)
    A = 'A',
    I = 'I',
    R = 'R',
    AFP = 'f',                          // AF'
};

class Registers {
public:
    static RegName parsePointerReg(const char *line);
    static RegName parseStackReg(const char *line);
    static RegName parseIndexReg(const char *line);
    static RegName parseIrReg(const char *line);
    static RegName parseDataReg(const char *line);

    static host::int_t encodePointerReg(RegName regName);
    static host::int_t encodeStackReg(RegName regName);
    static host::int_t encodeIndexReg(RegName regName);
    static host::int_t encodeIrReg(RegName regName);
    static host::int_t encodeDataReg(RegName regName);

    static RegName decodePointerReg(target::byte_t regNum);
    static RegName decodePointerRegIx(
        target::byte_t regNum, RegName ix);
    static RegName decodeStackReg(target::byte_t regNum);
    static RegName decodeIndexReg(target::byte_t regNum);
    static RegName decodeIrReg(target::byte_t regNum);
    static RegName decodeDataReg(target::byte_t regNum);

    static bool compareRegName(const char *line, RegName regName);
    static host::uint_t regNameLen(RegName regName);

    static char *outRegName(char *out, const RegName regName);
    static char *outCc4Name(char *out, target::opcode_t cc4);
    static char *outCc8Name(char *out, target::opcode_t cc8);

private:
    static RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end);
};

#endif // __REGISTER_Z80_H__
