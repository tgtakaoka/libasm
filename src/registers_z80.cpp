#include <ctype.h>

#include "registers_z80.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define ARRAY_RANGE(array) &(array)[0], &(array)[ARRAY_SIZE(array)]

static const RegName POINTER_REGS[] PROGMEM = { BC, DE, HL, SP };
static const RegName STACK_REGS[] PROGMEM = { BC, DE, HL, AF };
static const RegName INDEX_REGS[] PROGMEM = { BC, DE };
static const RegName IR_REGS[] PROGMEM = { I, R };
static const RegName DATA_REGS[] PROGMEM = { B, C, D, E, H, L, M, A };

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return c == regChar || (isalpha(c) && toupper(c) == regChar);
}

static char regName1stChar(const RegName regName) {
    switch (regName) {
    case HL:
    case H: return 'H';
    case BC:
    case B: return 'B';
    case DE:
    case D: return 'D';
    case SP: return 'S';
    case AF:
    case AFP:
    case A: return 'A';
    case IX:
    case IY:
    case I: return 'I';
    case C: return 'C';
    case E: return 'E';
    case L: return 'L';
    case M: return '(';
    case R: return 'R';
    default: return 0;
    }
}

static char regName2ndChar(const RegName regName) {
    switch (regName) {
    case HL: return 'L';
    case BC: return 'C';
    case DE: return 'E';
    case SP: return 'P';
    case AF:
    case AFP: return 'F';
    case IX: return 'X';
    case IY: return 'Y';
    case M: return 'H';
    default: return 0;
    }
}

static char regName3rdChar(const RegName regName) {
    switch (regName) {
    case M: return 'L';
    case AFP: return '\'';
    default: return 0;
    }
}

static char regName4thChar(const RegName regName) {
    return (regName == M) ? ')' : 0;
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
    switch (regName) {
    case M: return 4;
    case AFP: return 3;
    case HL:
    case BC:
    case DE:
    case SP:
    case AF:
    case IX:
    case IY: return 2;
    default: return 1;
    }
}

char *Registers::outRegName(char *out, const RegName regName) {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) {
        *out++ = r2;
        const char r3 = regName3rdChar(regName);
        if (r3) {
            *out++ = r3;
            const char r4 = regName4thChar(regName);
            if (r4) *out++ = r4;
        }
    }
    *out = 0;
    return out;
}

char *Registers::outCc4Name(char *out, const target::opcode_t cc4) {
    if ((cc4 & 1) == 0) *out++ = 'N';
    *out++ = (cc4 & 2) ? 'C' : 'Z';
    *out = 0;
    return out;
}

char *Registers::outCc8Name(char *out, const target::opcode_t cc8) {
    if (cc8 < 4) return outCc4Name(out, cc8);
    if (cc8 == 7) *out++ = 'M';
    else if (cc8 == 6) *out++ = 'P';
    else {
        *out++ = 'P';
        *out++ = (cc8 == 4) ? 'O' : 'E';
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

RegName Registers::parseIrReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(IR_REGS));
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

host::int_t Registers::encodeIrReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(IR_REGS));
}

host::int_t Registers::encodeDataReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(DATA_REGS));
}

RegName Registers::decodePointerReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(POINTER_REGS));
}

RegName Registers::decodePointerRegIx(target::byte_t regNum,
    RegName ix) {
    const RegName regName = decodeRegNumber(regNum, ARRAY_RANGE(POINTER_REGS));
    return regName == HL ? ix : regName;
}

RegName Registers::decodeStackReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(STACK_REGS));
}

RegName Registers::decodeIndexReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(INDEX_REGS));
}

RegName Registers::decodeIrReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(IR_REGS));
}

RegName Registers::decodeDataReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(DATA_REGS));
}
