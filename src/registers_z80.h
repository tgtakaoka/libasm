/* -*- mode: c++; -*- */
#ifndef __REGISTER_Z80_H__
#define __REGISTER_Z80_H__

enum RegName : char {
    REG_UNDEF = 0,
    REG_HL = 'h',
    REG_BC = 'b',
    REG_DE = 'd',
    REG_SP = 's',
    REG_AF = 'a',
    REG_AFP = 'f',              // AF'
    REG_IX = 'x',
    REG_IY = 'y',
    REG_B = 'B',
    REG_C = 'C',
    REG_D = 'D',
    REG_E = 'E',
    REG_H = 'H',
    REG_L = 'L',
    REG_A = 'A',
    REG_I = 'I',
    REG_R = 'R',
};

enum CcName : char {
    CC_UNDEF = 0,
    CC_Z  = 'Z',
    CC_NZ = 'z',
    CC_C  = 'C',
    CC_NC = 'c',
    CC_PO = 'o',
    CC_PE = 'e',
    CC_P  = 'P',
    CC_M  = 'M',
};

class Registers {
public:
    static CcName parseCc4Name(const char *line);
    static CcName parseCc8Name(const char *line);
    static bool compareRegName(const char *line, RegName regName);
    static host::uint_t ccNameLen(const CcName ccName);
    static host::int_t encodeCcName(CcName ccName);

    static RegName parseRegister(const char *line);
    static host::uint_t regNameLen(const RegName regName);
    static host::int_t encodePointerReg(RegName regName);
    static host::int_t encodePointerRegIx(RegName regName, RegName ix);
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

    static char *outRegName(char *out, const RegName regName);
    static char *outCc4Name(char *out, target::opcode_t cc4);
    static char *outCc8Name(char *out, target::opcode_t cc8);

private:
    static RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end);
};

#endif // __REGISTER_Z80_H__
