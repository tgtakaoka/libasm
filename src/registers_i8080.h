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

    static RegName decodePointerReg(uint8_t regNum);
    static RegName decodeStackReg(uint8_t regNum);
    static RegName decodeIndexReg(uint8_t regNum);
    static RegName decodeDataReg(uint8_t regNum);

    static bool compareRegName(const char *line, RegName regName);
    static host::uint_t regNameLen(RegName regName);

    static char *outRegName(char *out, const RegName regName);
    static char *outCCRBits(char *out, uint8_t val);

private:
    static RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end);
};

#endif // __REGISTER_I8080_H__
