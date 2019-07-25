#include <ctype.h>

#include "registers_i8080.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define ARRAY_RANGE(array) &(array)[0], &(array)[ARRAY_SIZE(array)]

static const RegName POINTER_REGS[] PROGMEM = { B, D, H, SP };
static const RegName STACK_REGS[] PROGMEM = { B, D, H, PSW };
static const RegName INDEX_REGS[] PROGMEM = { B, D };
static const RegName DATA_REGS[] PROGMEM = { B, C, D, E, H, L, M, A };

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
    if (regName == SP) return 'P';
    if (regName == PSW) return 'S';
    return 0;
}

static char regName3rdChar(const RegName regName) {
    if (regName == PSW) return 'W';
    return 0;
}

bool Registers::compareRegName(const char *line, RegName regName) {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    const char r3 = regName3rdChar(regName);
    if (r3 && !regCharCaseEqual(*line++, r3)) return false;
    return !isidchar(*line);
}

host::uint_t Registers::regNameLen(RegName regName) {
    return regName2ndChar(regName) == 0 ? 1
        : (regName3rdChar(regName) == 0 ? 2 : 3);
}

char *Registers::outRegName(char *out, const RegName regName) {
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

RegName Registers::parseRegName(
    const char *line, const RegName *table, const RegName *end) {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return NONE;
}

static RegName decodeRegNumber(
    const host::uint_t regNum, const RegName *table, const RegName *end) {
    const RegName *entry = &table[regNum];
    return entry < end ? RegName(pgm_read_byte(entry)) : NONE;
}

RegName Registers::parsePointerReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(POINTER_REGS));
}

RegName Registers::parseStackReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(STACK_REGS));
}

RegName Registers::parseIndexReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(INDEX_REGS));
}

RegName Registers::parseDataReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(DATA_REGS));
}

host::int_t Registers::encodePointerReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(POINTER_REGS));
}

host::int_t Registers::encodeStackReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(STACK_REGS));
}

host::int_t Registers::encodeIndexReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(INDEX_REGS));
}

host::int_t Registers::encodeDataReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(DATA_REGS));
}

RegName Registers::decodePointerReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(POINTER_REGS));
}

RegName Registers::decodeStackReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(STACK_REGS));
}

RegName Registers::decodeIndexReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(INDEX_REGS));
}

RegName Registers::decodeDataReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(DATA_REGS));
}
