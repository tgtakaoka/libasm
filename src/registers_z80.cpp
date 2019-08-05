#include <ctype.h>

#include "registers_z80.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define ARRAY_RANGE(array) &(array)[0], &(array)[ARRAY_SIZE(array)]

static constexpr RegName POINTER_REGS[] PROGMEM = { BC, DE, HL, SP };
static constexpr RegName STACK_REGS[] PROGMEM = { BC, DE, HL, AF };
static constexpr RegName INDEX_REGS[] PROGMEM = { BC, DE };
static constexpr RegName IR_REGS[] PROGMEM = { I, R };
static constexpr RegName DATA_REGS[] PROGMEM = { B, C, D, E, H, L, NONE, A };
static constexpr RegName NON_DATA_REGS[] PROGMEM = { SP, IX, IY, I, R, AFP };

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return c == regChar || toupper(c) == regChar;
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
    default: return 0;
    }
}

static char regName3rdChar(const RegName regName) {
    switch (regName) {
    case AFP: return '\'';
    default: return 0;
    }
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
        if (r3)
            *out++ = r3;
    }
    *out = 0;
    return out;
}

static constexpr CcName CC8_NAMES[] PROGMEM = {
    CC_NZ, CC_Z, CC_NC, CC_C, CC_PO, CC_PE, CC_P, CC_M
};

static bool ccCharCaseEqual(char c, char ccChar) {
    return c == ccChar || toupper(c) == ccChar;
}

static char ccName1stChar(const CcName ccName) {
    if (isupper(ccName)) return ccName;
    if (ccName == CC_NZ || ccName == CC_NC) return 'N';
    return 'P';
}

static char ccName2ndChar(const CcName ccName) {
    if (isupper(ccName)) return 0;
    return toupper(ccName);
}

static host::uint_t ccNameLen(const CcName ccName) {
    return isupper(ccName) ? 1 : 2;
}

static char *outCcName(char *out, const CcName ccName) {
    *out++ = ccName1stChar(ccName);
    const char c2 = ccName2ndChar(ccName);
    if (c2) *out++ = c2;
    *out = 0;
    return out;
}

static bool compareCcName(const char *line, CcName ccName) {
    if (!ccCharCaseEqual(*line++, ccName1stChar(ccName))) return false;
    const char c2 = ccName2ndChar(ccName);
    if (c2 && !ccCharCaseEqual(*line++, c2)) return false;
    return !isalpha(*line);
}

char *Registers::outCc4Name(char *out, const target::opcode_t cc4) {
    const CcName cc = CcName(pgm_read_byte(&CC8_NAMES[cc4 & 3]));
    return outCcName(out, cc);
}

char *Registers::outCc8Name(char *out, const target::opcode_t cc8) {
    const CcName cc = CcName(pgm_read_byte(&CC8_NAMES[cc8 & 7]));
    return outCcName(out, cc);
}

static const char *parseCcName(const char *line, target::int8_t &cc, host::uint_t max) {
    for (cc = 0; cc < max; cc++) {
        const CcName ccName = CcName(pgm_read_byte(&CC8_NAMES[cc]));
        if (compareCcName(line, ccName))
            return line + ccNameLen(ccName);
    }
    cc = -1;
    return line;
}

const char *Registers::parseCc4Name(const char *line, target::int8_t &cc4) {
    return parseCcName(line, cc4, 4);
}

const char *Registers::parseCc8Name(const char *line, target::int8_t &cc8) {
    return parseCcName(line, cc8, 8);
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

RegName Registers::parseRegister(const char *line) {
    RegName regName;
    if ((regName = parseRegName(line, ARRAY_RANGE(NON_DATA_REGS))) != NONE)
        return regName;
    if ((regName = parseRegName(line, ARRAY_RANGE(DATA_REGS))) != NONE)
        return regName;
    return parseRegName(line, ARRAY_RANGE(STACK_REGS));
}

host::int_t Registers::encodePointerReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(POINTER_REGS));
}

host::int_t Registers::encodePointerRegIx(RegName regName, RegName ix) {
    if (regName == ix) regName = HL;
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
    if (regName == HL) regName = NONE;
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
