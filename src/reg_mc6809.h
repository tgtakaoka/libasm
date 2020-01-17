/* -*- mode: c++; -*- */
#ifndef __REG_MC6809_H__
#define __REG_MC6809_H__

enum RegName : char {
    REG_UNDEF = 0,
    OFFSET = 'o',
    REG_D = 'D',
    REG_X = 'X',
    REG_Y = 'Y',
    REG_U = 'U',
    REG_S = 'S',
    REG_PC = 'P',
    REG_PCR = 'p', // Program counter relative addressing
    REG_A = 'A',
    REG_B = 'B',
    REG_CC = 'C',
    REG_DP = 'd',
    REG_E = 'E',
    REG_F = 'F',
    REG_W = 'W',
    REG_V = 'V',
    REG_ZERO = '0',
};

class RegMc6809 {
public:
    McuType _mcuType;

    RegName parseIndexReg(const char *line) const;
    RegName parseBaseReg(const char *line) const;
    RegName parseDataReg(const char *line) const;
    host::int_t encodeIndexReg(RegName regName) const;
    host::int_t encodeBaseReg(RegName regName) const;
    host::int_t encodeDataReg(RegName regName) const;
    RegName decodeIndexReg(uint8_t regNum) const;
    RegName decodeBaseReg(uint8_t regNum) const;
    RegName decodeRegName(uint8_t regNum) const;

    static char *outRegName(char *out, const RegName regName);
    static char *outCCRBits(char *out, uint8_t val);

    static RegName getStackReg(host::uint_t bit, target::insn_t insnCode);
    static bool compareRegName(const char *line, RegName regName);
    static host::uint_t regNameLen(RegName regName);

    static RegName parseBitOpReg(const char *line);
    static host::int_t encodeBitOpReg(RegName regName);

    static RegName parseTfmBaseReg(const char *line);
    static host::int_t encodeTfmBaseReg(RegName regName);
    static RegName decodeBitOpReg(uint8_t regNum);
    static RegName decodeTfmBaseReg(uint8_t regNum);
    static char tfmSrcModeChar(host::uint_t mode);
    static char tfmDstModeChar(host::uint_t mode);

private:
    static host::int_t encodeRegNumber(
        RegName regName, const RegName *table, const RegName *end);
    static RegName parseRegName(
        const char *line, const RegName *table, const RegName *end);
    static RegName decodeRegNumber(
        host::uint_t regNum, const RegName *table, const RegName *end);

    static bool isidchar(const char c);
    static bool regCharCaseEqual(char c, char regChar);
    static char regName1stChar(const RegName);
    static char regName2ndChar(const RegName);
    static char regName3rdChar(const RegName);
};

#endif // __REG_MC6809_H__
