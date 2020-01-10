/* -*- mode: c++; -*- */
#ifndef __REGISTER_HD6309_H__
#define __REGISTER_HD6309_H__

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

class RegHd6309Utils {
public:
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

template<McuType type> struct IndexRegHd6309 {
    const RegName *begin() const;
    const RegName *end() const;
};
template<McuType type> struct BaseRegHd6309 {
    const RegName *begin() const;
    const RegName *end() const;
};
template<McuType type> struct DataRegHd6309 {
    const RegName *begin() const;
    const RegName *end() const;
};

template<McuType mcuType = HD6309>
class RegHd6309 : public RegHd6309Utils {
public:
    RegHd6309() : _indexRegHd6309(), _baseRegHd6309(), _dataRegHd6309() {}

    RegName parseIndexReg(const char *line) const {
        return parseRegName(line, _indexRegHd6309.begin(), _indexRegHd6309.end());
    }
    RegName parseBaseReg(const char *line) const {
        return parseRegName(line, _baseRegHd6309.begin(), _baseRegHd6309.end());
    }
    RegName parseDataReg(const char *line) const {
        return parseRegName(line, _dataRegHd6309.begin(), _dataRegHd6309.end());
    }
    host::int_t encodeIndexReg(RegName regName) const {
        return encodeRegNumber(regName, _indexRegHd6309.begin(), _indexRegHd6309.end());
    }
    host::int_t encodeBaseReg(RegName regName) const {
        return encodeRegNumber(regName, _baseRegHd6309.begin(), _baseRegHd6309.end());
    }
    host::int_t encodeDataReg(RegName regName) const {
        return encodeRegNumber(regName, _dataRegHd6309.begin(), _dataRegHd6309.end());
    }
    RegName decodeIndexReg(uint8_t regNum) const {
        return decodeRegNumber(regNum, _indexRegHd6309.begin(), _indexRegHd6309.end());
    }
    RegName decodeBaseReg(uint8_t regNum) const {
        return decodeRegNumber(regNum, _baseRegHd6309.begin(), _baseRegHd6309.end());
    }
    RegName decodeRegName(uint8_t regNum) const {
        return decodeRegNumber(regNum, _dataRegHd6309.begin(), _dataRegHd6309.end());
    }

private:
    const IndexRegHd6309<mcuType> _indexRegHd6309;
    const BaseRegHd6309<mcuType> _baseRegHd6309;
    const DataRegHd6309<mcuType> _dataRegHd6309;
};

#endif // __REGISTER_HD6309_H__
