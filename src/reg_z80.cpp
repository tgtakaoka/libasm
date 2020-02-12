#include "config_z80.h"
#include "reg_z80.h"
#include "table_z80.h"
#include "reg_z80.h"

#include <ctype.h>

static constexpr RegName ALL_REGS[] PROGMEM = {
    REG_AFP, REG_AF, REG_HL, REG_BC, REG_DE, REG_SP, REG_IX, REG_IY,
    REG_A, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, REG_I, REG_R,
};

static constexpr RegName POINTER_REGS[] PROGMEM = {
    REG_BC, REG_DE, REG_HL, REG_SP
};
static constexpr RegName STACK_REGS[] PROGMEM = {
    REG_BC, REG_DE, REG_HL, REG_AF
};
static constexpr RegName INDIRECT_BASES[] PROGMEM = {
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
    return toupper(c) == toupper(regChar);
}

char RegZ80::regName1stChar(const RegName regName) const {
    if (regName == REG_IX || regName == REG_IY)
        return _uppercase ? 'I' : 'i';
    if (regName == REG_AFP)
        return _uppercase ? 'A' : 'a';
    const char r = char(regName);
    return _uppercase ? toupper(r) : tolower(r);
}

char RegZ80::regName2ndChar(const RegName regName) const {
    char c;
    switch (regName) {
    case REG_HL: c = 'L'; break;
    case REG_BC: c = 'C'; break;
    case REG_DE: c = 'E'; break;
    case REG_SP: c = 'P'; break;
    case REG_AF:
    case REG_AFP: c = 'F'; break;
    case REG_IX: c = 'X'; break;
    case REG_IY: c = 'Y'; break;
    default: c = 0;
    }
    return _uppercase ? c : tolower(c);
}

char RegZ80::regName3rdChar(const RegName regName) const {
    switch (regName) {
    case REG_AFP: return '\'';
    default: return 0;
    }
}

bool RegZ80::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    const char r3 = regName3rdChar(regName);
    if (r3 && !regCharCaseEqual(*line++, r3)) return false;
    return !isidchar(*line);
}

host::uint_t RegZ80::regNameLen(const RegName regName) {
    if (regName == REG_AFP) return 3;
    if (isupper(char(regName))) return 1;
    return 2;
}

char *RegZ80::outRegName(char *out, const RegName regName) const {
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
    return toupper(c) == toupper(ccChar);
}

char RegZ80::ccName1stChar(const CcName ccName) const {
    const char cc = char(ccName);
    if (isupper(cc))
        return _uppercase ? cc : tolower(cc);
    if (ccName == CC_NZ || ccName == CC_NC)
        return _uppercase ? 'N' : 'n';
    return _uppercase ? 'P' : 'p';
}

char RegZ80::ccName2ndChar(const CcName ccName) const {
    if (isupper(ccName)) return 0;
    return _uppercase ? toupper(ccName) : char(ccName);
}

host::uint_t RegZ80::ccNameLen(const CcName ccName) {
    return isupper(ccName) ? 1 : 2;
}

char *RegZ80::outCcName(char *out, const CcName ccName) const {
    *out++ = ccName1stChar(ccName);
    const char c2 = ccName2ndChar(ccName);
    if (c2) *out++ = c2;
    *out = 0;
    return out;
}

bool RegZ80::compareCcName(const char *line, CcName ccName) const {
    if (!ccCharCaseEqual(*line++, ccName1stChar(ccName))) return false;
    const char c2 = ccName2ndChar(ccName);
    if (c2 && !ccCharCaseEqual(*line++, c2)) return false;
    return !isalpha(*line);
}

char *RegZ80::outCc4Name(char *out, const target::opcode_t cc4) const {
    const CcName cc = CcName(pgm_read_byte(&CC8_NAMES[cc4 & 3]));
    return outCcName(out, cc);
}

char *RegZ80::outCc8Name(char *out, const target::opcode_t cc8) const {
    const CcName cc = CcName(pgm_read_byte(&CC8_NAMES[cc8 & 7]));
    return outCcName(out, cc);
}

CcName RegZ80::parseCcName(const char *line, host::int_t max) const {
    for (host::int_t cc = 0; cc < max; cc++) {
        const CcName ccName = CcName(pgm_read_byte(&CC8_NAMES[cc]));
        if (compareCcName(line, ccName))
            return ccName;
    }
    return CC_UNDEF;
}

CcName RegZ80::parseCc4Name(const char *line) const {
    return parseCcName(line, 4);
}

CcName RegZ80::parseCc8Name(const char *line) const {
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
    const char *line, const RegName *table, const RegName *end) const {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return REG_UNDEF;
}

OprSize RegZ80::registerSize(const RegName regName) {
    switch (regName) {
    case REG_BC: case REG_DE: case REG_HL:
    case REG_SP: case REG_IX: case REG_IY:
    case REG_AF: case REG_AFP:
        return SZ_WORD;
    case REG_B: case REG_C: case REG_D: case REG_E:
    case REG_H: case REG_L: case REG_A:
    case REG_I: case REG_R:
        return SZ_BYTE;
    default:
        return SZ_NONE;
    }
}

static RegName decodeRegNumber(
    const host::uint_t regNum, const RegName *table, const RegName *end) {
    const RegName *entry = &table[regNum];
    return entry < end ? RegName(pgm_read_byte(entry)) : REG_UNDEF;
}

RegName RegZ80::parseRegister(const char *line) const {
    return parseRegName(line, ARRAY_RANGE(ALL_REGS));
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

host::int_t RegZ80::encodeIndirectBase(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(INDIRECT_BASES));
}

void RegZ80::encodeIndexReg(Insn &insn, RegName ixReg) {
    const target::opcode_t prefix =
        (ixReg == REG_IX) ? TableZ80::PREFIX_IX : TableZ80::PREFIX_IY;
    insn.setInsnCode(prefix, insn.opCode());
}

host::int_t RegZ80::encodeIrReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(IR_REGS));
}

host::int_t RegZ80::encodeDataReg(RegName regName) {
    // (HL) is parsed as HL_PTR, then looked up as format=REG_8
    // reg=REG_HL.  So we have to map REG_HL to register number 6
    // which is encoded by REG_UNDEF in DATA_REG[].
    if (regName == REG_HL) regName = REG_UNDEF;
    return encodeRegNumber(regName, ARRAY_RANGE(DATA_REGS));
}

RegName RegZ80::decodePointerReg(uint8_t regNum, const Insn *insn) {
    const RegName regName = decodeRegNumber(regNum & 3, ARRAY_RANGE(POINTER_REGS));
    if (insn == nullptr || regName != REG_HL) return regName;
    return decodeIndexReg(*insn);
}

RegName RegZ80::decodeStackReg(uint8_t regNum) {
    return decodeRegNumber(regNum & 3, ARRAY_RANGE(STACK_REGS));
}

RegName RegZ80::decodeIndirectBase(uint8_t regNum) {
    return decodeRegNumber(regNum & 1, ARRAY_RANGE(INDIRECT_BASES));
}

RegName RegZ80::decodeIndexReg(const Insn &insn) {
    const target::opcode_t prefix = insn.prefixCode();
    if (prefix == TableZ80::PREFIX_IX) return REG_IX;
    if (prefix == TableZ80::PREFIX_IY) return REG_IY;
    return REG_UNDEF;
}

RegName RegZ80::decodeIrReg(uint8_t regNum) {
    return decodeRegNumber(regNum & 1, ARRAY_RANGE(IR_REGS));
}

RegName RegZ80::decodeDataReg(uint8_t regNum) {
    return decodeRegNumber(regNum & 7, ARRAY_RANGE(DATA_REGS));
}
