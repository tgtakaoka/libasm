#include <ctype.h>

#include "config_z80.h"

static constexpr RegName POINTER_REGS[] PROGMEM = {
    REG_BC, REG_DE, REG_HL, REG_SP
};
static constexpr RegName STACK_REGS[] PROGMEM = {
    REG_BC, REG_DE, REG_HL, REG_AF
};
static constexpr RegName INDEX_REGS[] PROGMEM = {
    REG_BC, REG_DE
};
static constexpr RegName IR_REGS[] PROGMEM = {
    REG_I, REG_R
};
static constexpr RegName DATA_REGS[] PROGMEM = {
    REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, REG_UNDEF, REG_A
};
static constexpr RegName NON_DATA_REGS[] PROGMEM = {
    REG_SP, REG_IX, REG_IY, REG_I, REG_R, REG_AFP
};

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return c == regChar || toupper(c) == regChar;
}

static char regName1stChar(const RegName regName) {
    switch (regName) {
    case REG_HL:
    case REG_H: return 'H';
    case REG_BC:
    case REG_B: return 'B';
    case REG_DE:
    case REG_D: return 'D';
    case REG_SP: return 'S';
    case REG_AF:
    case REG_AFP:
    case REG_A: return 'A';
    case REG_IX:
    case REG_IY:
    case REG_I: return 'I';
    case REG_C: return 'C';
    case REG_E: return 'E';
    case REG_L: return 'L';
    case REG_R: return 'R';
    default: return 0;
    }
}

static char regName2ndChar(const RegName regName) {
    switch (regName) {
    case REG_HL: return 'L';
    case REG_BC: return 'C';
    case REG_DE: return 'E';
    case REG_SP: return 'P';
    case REG_AF:
    case REG_AFP: return 'F';
    case REG_IX: return 'X';
    case REG_IY: return 'Y';
    default: return 0;
    }
}

static char regName3rdChar(const RegName regName) {
    switch (regName) {
    case REG_AFP: return '\'';
    default: return 0;
    }
}

bool RegZ80::compareRegName(const char *line, RegName regName) {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    const char r3 = regName3rdChar(regName);
    if (r3 && !regCharCaseEqual(*line++, r3)) return false;
    return !isidchar(*line);
}

host::uint_t RegZ80::regNameLen(const RegName regName) {
    switch (regName) {
    case REG_AFP: return 3;
    case REG_HL:
    case REG_BC:
    case REG_DE:
    case REG_SP:
    case REG_AF:
    case REG_IX:
    case REG_IY: return 2;
    default: return 1;
    }
}

char *RegZ80::outRegName(char *out, const RegName regName) {
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

host::uint_t RegZ80::ccNameLen(const CcName ccName) {
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

char *RegZ80::outCc4Name(char *out, const target::opcode_t cc4) {
    const CcName cc = CcName(pgm_read_byte(&CC8_NAMES[cc4 & 3]));
    return outCcName(out, cc);
}

char *RegZ80::outCc8Name(char *out, const target::opcode_t cc8) {
    const CcName cc = CcName(pgm_read_byte(&CC8_NAMES[cc8 & 7]));
    return outCcName(out, cc);
}

static CcName parseCcName(const char *line, host::int_t max) {
    for (host::int_t cc = 0; cc < max; cc++) {
        const CcName ccName = CcName(pgm_read_byte(&CC8_NAMES[cc]));
        if (compareCcName(line, ccName))
            return ccName;
    }
    return CC_UNDEF;
}

CcName RegZ80::parseCc4Name(const char *line) {
    return parseCcName(line, 4);
}

CcName RegZ80::parseCc8Name(const char *line) {
    return parseCcName(line, 8);
}

host::int_t RegZ80::encodeCcName(const CcName ccName) {
    for (host::int_t cc = 0; cc < 8; cc++) {
        if (CcName(pgm_read_byte(&CC8_NAMES[cc])) == ccName)
            return cc;
    }
    return -1;
}

static host::int_t encodeRegNumber(
    RegName regName, const RegName *table, const RegName *end) {
    for (const RegName *p = table; p < end; p++) {
        if (pgm_read_byte(p) == regName) return p - table;
    }
    return -1;
}

RegName RegZ80::parseRegName(
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

RegName RegZ80::parseRegister(const char *line) {
    RegName regName;
    if ((regName = parseRegName(line, ARRAY_RANGE(DATA_REGS))) != REG_UNDEF)
        return regName;
    if ((regName = parseRegName(line, ARRAY_RANGE(STACK_REGS))) != REG_UNDEF)
        return regName;
    return parseRegName(line, ARRAY_RANGE(NON_DATA_REGS));
}

host::int_t RegZ80::encodePointerReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(POINTER_REGS));
}

host::int_t RegZ80::encodePointerRegIx(RegName regName, RegName ix) {
    if (regName == ix) regName = REG_HL;
    return encodeRegNumber(regName, ARRAY_RANGE(POINTER_REGS));
}

host::int_t RegZ80::encodeStackReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(STACK_REGS));
}

host::int_t RegZ80::encodeIndexReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(INDEX_REGS));
}

host::int_t RegZ80::encodeIrReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(IR_REGS));
}

host::int_t RegZ80::encodeDataReg(RegName regName) {
    if (regName == REG_HL) regName = REG_UNDEF;
    return encodeRegNumber(regName, ARRAY_RANGE(DATA_REGS));
}

RegName RegZ80::decodePointerReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(POINTER_REGS));
}

RegName RegZ80::decodePointerRegIx(uint8_t regNum,
    RegName ix) {
    const RegName regName = decodeRegNumber(regNum, ARRAY_RANGE(POINTER_REGS));
    return regName == REG_HL ? ix : regName;
}

RegName RegZ80::decodeStackReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(STACK_REGS));
}

RegName RegZ80::decodeIndexReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(INDEX_REGS));
}

RegName RegZ80::decodeIrReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(IR_REGS));
}

RegName RegZ80::decodeDataReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(DATA_REGS));
}
