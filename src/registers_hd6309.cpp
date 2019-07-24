#include <ctype.h>

#include "registers_hd6309.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define ARRAY_RANGE(array) &(array)[0], &(array)[ARRAY_SIZE(array)]

static const RegName MC6809_INDEX_REGS[] PROGMEM = {
    NONE, NONE, NONE, NONE, NONE, B, A, NONE, NONE, NONE, NONE, D
};
static const RegName HD6309_INDEX_REGS[] PROGMEM = {
    NONE, NONE, NONE, NONE, NONE, B, A, E, NONE, NONE, F, D, NONE, NONE, W
};
static const RegName MC6809_BASE_REGS[] PROGMEM = { X, Y, U, S };
static const RegName HD6309_BASE_REGS[] PROGMEM = { X, Y, U, S, W };
static const RegName MC6809_DATA_REGS[] PROGMEM = {
    D, X, Y, U, S, PC, NONE, NONE, A, B, CC, DP
};
static const RegName HD6309_DATA_REGS[] PROGMEM = {
    D, X, Y, U, S, PC, W, V, A, B, CC, DP, ZERO, ZERO, E, F,
};

static const RegName STACK_S_REGS[8] PROGMEM = { CC, A, B, DP, X, Y, U, PC };
static const RegName STACK_U_REGS[8] PROGMEM = { CC, A, B, DP, X, Y, S, PC };
static const char CCR_BITS[8] PROGMEM = { 'E', 'F', 'H', 'I', 'N', 'Z', 'V', 'C' };
static const RegName BIT_OP_REGS[] PROGMEM = { CC, A, B, NONE };

static const RegName TFM_BASE_REGS[] PROGMEM = { D, X, Y, U, S };
static const char TFM_SRC_MODES[4] PROGMEM = { '+', '-', '+',   0 };
static const char TFM_DST_MODES[4] PROGMEM = { '+', '-',   0, '+' };

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return c == regChar || (isalpha(c) && toupper(c) == regChar);
}

static char regName1stChar(const RegName regName) {
    return (regName == DP) ? 'D' : char(regName);
}

static char regName2ndChar(const RegName regName) {
    if (regName == PC) return 'C';
    if (regName == CC) return 'C';
    if (regName == DP) return 'P';
    return 0;
}

bool Registers::compareRegName(const char *line, RegName regName) {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    return !isidchar(*line);
}

host::uint_t Registers::regNameLen(RegName regName) {
    return regName2ndChar(regName) == 0 ? 1 : 2;
}

char *Registers::outRegName(char *out, const RegName regName) {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) *out++ = r2;
    *out = 0;
    return out;
}

char *Registers::outCCRBits(char *out, target::byte_t val) {
    host::int_t n = 0;
    for (host::uint_t mask = 0x80, i = 0; i < 8; mask >>= 1, i++) {
        if (val & mask) {
            if (n == 1) { char b = *--out; *out++ = '('; *out++ = b; }
            if (n) *out++ = '|';
            *out++ = pgm_read_byte(&CCR_BITS[i]);
            n++;
        }
    }
    if (n >= 2) *out++ = ')';
    *out = 0;
    return out;
}

host::int_t Registers::encodeRegNumber(
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

RegName Registers::getStackReg(host::uint_t bit, target::insn_t insnCode) {
    const RegName *table = (insnCode & 2) == 0
        ? &STACK_S_REGS[0] : &STACK_U_REGS[0];
    return RegName(pgm_read_byte(&table[bit]));
}

RegName Registers::parseDataReg(const char *line) const {
    return isMc6809()
        ? parseRegName(line, ARRAY_RANGE(MC6809_DATA_REGS))
        : parseRegName(line, ARRAY_RANGE(HD6309_DATA_REGS));
}

RegName Registers::parseIndexReg(const char *line) const {
    return isMc6809()
        ? parseRegName(line, ARRAY_RANGE(MC6809_INDEX_REGS))
        : parseRegName(line, ARRAY_RANGE(HD6309_INDEX_REGS));
}

RegName Registers::parseBaseReg(const char *line) const {
    return isMc6809()
        ? parseRegName(line, ARRAY_RANGE(MC6809_BASE_REGS))
        : parseRegName(line, ARRAY_RANGE(HD6309_BASE_REGS));
}

RegName Registers::parseBitOpReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(BIT_OP_REGS));
}

RegName Registers::parseTfmBaseReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(TFM_BASE_REGS));
}

host::int_t Registers::encodeDataReg(RegName regName) const {
    return isMc6809()
        ? encodeRegNumber(regName, ARRAY_RANGE(MC6809_DATA_REGS))
        : encodeRegNumber(regName, ARRAY_RANGE(HD6309_DATA_REGS));
}

host::int_t Registers::encodeIndexReg(RegName regName) const {
    return isMc6809()
        ? encodeRegNumber(regName, ARRAY_RANGE(MC6809_INDEX_REGS))
        : encodeRegNumber(regName, ARRAY_RANGE(HD6309_INDEX_REGS));
}

host::int_t Registers::encodeBaseReg(RegName regName) const {
    return isMc6809()
        ? encodeRegNumber(regName, ARRAY_RANGE(MC6809_BASE_REGS))
        : encodeRegNumber(regName, ARRAY_RANGE(HD6309_BASE_REGS));
}

host::int_t Registers::encodeBitOpReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(BIT_OP_REGS));
}

host::int_t Registers::encodeTfmBaseReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(TFM_BASE_REGS));
}

RegName Registers::decodeIndexReg(target::byte_t regNum) const {
    const host::uint_t num = isMc6809() 
        ? ARRAY_SIZE(MC6809_INDEX_REGS) : ARRAY_SIZE(HD6309_INDEX_REGS);
    if (regNum >= num) return NONE;
    const RegName *table = isMc6809()
        ? &MC6809_INDEX_REGS[0] : &HD6309_INDEX_REGS[0];
    return RegName(pgm_read_byte(&table[regNum]));
}

RegName Registers::decodeBaseReg(target::byte_t regNum) const {
    if (regNum >= ARRAY_SIZE(MC6809_BASE_REGS)) return NONE;
    return RegName(pgm_read_byte(&MC6809_BASE_REGS[regNum]));
}

RegName Registers::decodeRegName(target::byte_t regNum) const {
    const host::uint_t num = isMc6809()
        ? ARRAY_SIZE(MC6809_DATA_REGS) : ARRAY_SIZE(HD6309_DATA_REGS);
    if (regNum >= num) return NONE;
    const RegName *table = isMc6809()
        ? &MC6809_DATA_REGS[0] : &HD6309_DATA_REGS[0];
    return RegName(pgm_read_byte(&table[regNum]));
}

RegName Registers::decodeBitOpReg(target::byte_t regNum) {
    if (regNum >= ARRAY_SIZE(BIT_OP_REGS)) return NONE;
    return RegName(pgm_read_byte(&BIT_OP_REGS[regNum]));
}

RegName Registers::decodeTfmBaseReg(target::byte_t regNum) {
    if (regNum >= ARRAY_SIZE(TFM_BASE_REGS)) return NONE;
    return RegName(pgm_read_byte(&TFM_BASE_REGS[regNum]));
}

char Registers::tfmSrcModeChar(host::uint_t mode) {
    return pgm_read_byte(&TFM_SRC_MODES[mode]);
}

char Registers::tfmDstModeChar(host::uint_t mode) {
    return pgm_read_byte(&TFM_DST_MODES[mode]);
}
