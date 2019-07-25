/* -*- mode: c++; -*- */
#ifndef __REGISTER_HD6309_H__
#define __REGISTER_HD6309_H__

#include "config_hd6309.h"

enum RegName : char {
    NONE = 0,
    OFFSET = 'o',
    D = 'D',
    X = 'X',
    Y = 'Y',
    U = 'U',
    S = 'S',
    PC = 'P',
    A = 'A',
    B = 'B',
    CC = 'C',
    DP = 'd',
    E = 'E',
    F = 'F',
    W = 'W',
    V = 'V',
    ZERO = '0',
};

class Registers {
public:
    Registers(McuMode mcuMode = HD6309)
        : _mcuMode(mcuMode) {}
    void setMcuMode(McuMode mcuMode) { _mcuMode = mcuMode; }
    bool isMc6809() const { return _mcuMode == MC6809; }
    bool isHd6309() const { return _mcuMode == HD6309; }

    RegName parseDataReg(const char *line) const;
    RegName parseIndexReg(const char *line) const;
    RegName parseBaseReg(const char *line) const;
    host::int_t encodeDataReg(RegName regName) const;
    host::int_t encodeIndexReg(RegName regName) const;
    host::int_t encodeBaseReg(RegName regName) const;
    RegName decodeIndexReg(target::byte_t regNum) const;
    RegName decodeBaseReg(target::byte_t regNum) const;
    RegName decodeRegName(target::byte_t regNum) const;

    static bool compareRegName(const char *line, RegName regName);
    static host::uint_t regNameLen(RegName regName);

    static char *outRegName(char *out, const RegName regName);
    static char *outCCRBits(char *out, target::byte_t val);

    static RegName getStackReg(host::uint_t bit, target::insn_t insnCode);
    static RegName parseBitOpReg(const char *line);
    static RegName parseTfmBaseReg(const char *line);
    static host::int_t encodeBitOpReg(RegName regName);
    static host::int_t encodeTfmBaseReg(RegName regName);
    static RegName decodeBitOpReg(target::byte_t regNum);
    static RegName decodeTfmBaseReg(target::byte_t regNum);

    static char tfmSrcModeChar(host::uint_t mode);
    static char tfmDstModeChar(host::uint_t mode);

private:
    McuMode _mcuMode;

    static RegName parseRegName(
        const char *line, const RegName *tabel, const RegName *end);
};

#endif // __REGISTER_HD6309_H__
