#include <ctype.h>

#include "config_i8080.h"

static constexpr RegName ALL_REGS[] PROGMEM = {
    REG_A, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, REG_SP, REG_PSW
};

static constexpr RegName POINTER_REGS[] PROGMEM = {
    REG_B, REG_D, REG_H, REG_SP
};
static constexpr RegName STACK_REGS[] PROGMEM = {
    REG_B, REG_D, REG_H, REG_PSW
};
static constexpr RegName INDEX_REGS[] PROGMEM = {
    REG_B, REG_D
};
static constexpr RegName DATA_REGS[] PROGMEM = {
    REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, REG_M, REG_A
};

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return c == regChar || (isalpha(c) && toupper(c) == regChar);
}

static char regName1stChar(const RegName regName) {
    return char(regName);
}

static char regName2ndChar(const RegName regName) {
    if (regName == REG_SP) return 'P';
    if (regName == REG_PSW) return 'S';
    return 0;
}

static char regName3rdChar(const RegName regName) {
    if (regName == REG_PSW) return 'W';
    return 0;
}

bool RegI8080::compareRegName(const char *line, RegName regName) {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    const char r3 = regName3rdChar(regName);
    if (r3 && !regCharCaseEqual(*line++, r3)) return false;
    return !isidchar(*line);
}

host::uint_t RegI8080::regNameLen(RegName regName) {
    return regName2ndChar(regName) == 0 ? 1
        : (regName3rdChar(regName) == 0 ? 2 : 3);
}

char *RegI8080::outRegName(char *out, const RegName regName) {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) {
        *out++ = r2;
        const char r3 = regName3rdChar(regName);
        if (r3) *out++ = r3;
    }
    *out = 0;
    return out;
}

static host::int_t encodeRegNumber(
    RegName regName, const RegName *table, const RegName *end) {
    for (const RegName *p = table; p < end; p++) {
        if (pgm_read_byte(p) == regName) return p - table;
    }
    return -1;
}

RegName RegI8080::parseRegName(
    const char *line, const RegName *table, const RegName *end) {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return REG_UNDEF;
}

static RegName decodeRegNumber(
    const host::uint_t regNum, const RegName *table, const RegName *end) {
    const RegName *entry = &table[regNum];
    return entry < end ? RegName(pgm_read_byte(entry)) : REG_UNDEF;
}

RegName RegI8080::parseRegister(const char *line) {
    return parseRegName(line, ARRAY_RANGE(ALL_REGS));
}

RegName RegI8080::parsePointerReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(POINTER_REGS));
}

RegName RegI8080::parseStackReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(STACK_REGS));
}

RegName RegI8080::parseIndexReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(INDEX_REGS));
}

RegName RegI8080::parseDataReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(DATA_REGS));
}

host::int_t RegI8080::encodePointerReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(POINTER_REGS));
}

host::int_t RegI8080::encodeStackReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(STACK_REGS));
}

host::int_t RegI8080::encodeIndexReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(INDEX_REGS));
}

host::int_t RegI8080::encodeDataReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(DATA_REGS));
}

RegName RegI8080::decodePointerReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(POINTER_REGS));
}

RegName RegI8080::decodeStackReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(STACK_REGS));
}

RegName RegI8080::decodeIndexReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(INDEX_REGS));
}

RegName RegI8080::decodeDataReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(DATA_REGS));
}
