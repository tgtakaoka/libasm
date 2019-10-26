/* -*- mode: c++; -*- */
#ifndef __REGISTER_HD6309_H__
#define __REGISTER_HD6309_H__

#include "config_hd6309.h"

enum RegName : char {
    REG_UNDEF = 0,
    OFFSET = 'o',
    REG_D = 'D',
    REG_X = 'X',
    REG_Y = 'Y',
    REG_U = 'U',
    REG_S = 'S',
    REG_PC = 'P',
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

class RegistersUtils {
public:
    static char *outRegName(char *out, const RegName regName);
    static char *outCCRBits(char *out, target::byte_t val);

    static RegName getStackReg(host::uint_t bit, target::insn_t insnCode);
    static bool compareRegName(const char *line, RegName regName);
    static host::uint_t regNameLen(RegName regName);

    static RegName parseBitOpReg(const char *line);
    static host::int_t encodeBitOpReg(RegName regName);

    static RegName parseTfmBaseReg(const char *line);
    static host::int_t encodeTfmBaseReg(RegName regName);
    static RegName decodeBitOpReg(target::byte_t regNum);
    static RegName decodeTfmBaseReg(target::byte_t regNum);
    static char tfmSrcModeChar(host::uint_t mode);
    static char tfmDstModeChar(host::uint_t mode);

protected:
    static host::int_t encodeRegNumber(
        RegName regName, const RegName *table, const RegName *end);
    static RegName parseRegName(
        const char *line, const RegName *table, const RegName *end);
    static RegName decodeRegNumber(
        host::uint_t regNum, const RegName *table, const RegName *end);

private:
    static bool isidchar(const char c);
    static bool regCharCaseEqual(char c, char regChar);
    static char regName1stChar(const RegName);
    static char regName2ndChar(const RegName);
};

template<McuType type> struct IndexRegisters {
    const RegName *begin() const;
    const RegName *end() const;
};
template<McuType type> struct BaseRegisters {
    const RegName *begin() const;
    const RegName *end() const;
};
template<McuType type> struct DataRegisters {
    const RegName *begin() const;
    const RegName *end() const;
};

template<McuType mcuType = HD6309>
class Registers : public RegistersUtils {
public:
    Registers() : _indexRegisters(), _baseRegisters(), _dataRegisters() {}

    RegName parseIndexReg(const char *line) const {
        return parseRegName(line, _indexRegisters.begin(), _indexRegisters.end());
    }
    RegName parseBaseReg(const char *line) const {
        return parseRegName(line, _baseRegisters.begin(), _baseRegisters.end());
    }
    RegName parseDataReg(const char *line) const {
        return parseRegName(line, _dataRegisters.begin(), _dataRegisters.end());
    }
    host::int_t encodeIndexReg(RegName regName) const {
        return encodeRegNumber(regName, _indexRegisters.begin(), _indexRegisters.end());
    }
    host::int_t encodeBaseReg(RegName regName) const {
        return encodeRegNumber(regName, _baseRegisters.begin(), _baseRegisters.end());
    }
    host::int_t encodeDataReg(RegName regName) const {
        return encodeRegNumber(regName, _dataRegisters.begin(), _dataRegisters.end());
    }
    RegName decodeIndexReg(target::byte_t regNum) const {
        return decodeRegNumber(regNum, _indexRegisters.begin(), _indexRegisters.end());
    }
    RegName decodeBaseReg(target::byte_t regNum) const {
        return decodeRegNumber(regNum, _baseRegisters.begin(), _baseRegisters.end());
    }
    RegName decodeRegName(target::byte_t regNum) const {
        return decodeRegNumber(regNum, _dataRegisters.begin(), _dataRegisters.end());
    }

private:
    const IndexRegisters<mcuType> _indexRegisters;
    const BaseRegisters<mcuType> _baseRegisters;
    const DataRegisters<mcuType> _dataRegisters;
};

#endif // __REGISTER_HD6309_H__
