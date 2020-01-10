#include <ctype.h>

#include "config_hd6309.h"

static constexpr RegName EXTRA_HD6309[] PROGMEM = {
    REG_E, REG_F, REG_V, REG_W
};
static constexpr RegName STACK_S_REGS[8] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_DP, REG_X, REG_Y, REG_U, REG_PC
};
static constexpr RegName STACK_U_REGS[8] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_DP, REG_X, REG_Y, REG_S, REG_PC
};
static constexpr char CCR_BITS[8] PROGMEM = {
    'E', 'F', 'H', 'I', 'N', 'Z', 'V', 'C'
};
static constexpr RegName BIT_OP_REGS[] PROGMEM = {
    REG_CC, REG_A, REG_B, REG_UNDEF
};

static constexpr RegName TFM_BASE_REGS[] PROGMEM = {
    REG_D, REG_X, REG_Y, REG_U, REG_S
};
static constexpr char TFM_SRC_MODES[4] PROGMEM = {
    '+', '-', '+',   0
};
static constexpr char TFM_DST_MODES[4] PROGMEM = {
    '+', '-',   0, '+'
};

bool RegHd6309Utils::isidchar(const char c) {
    return isalnum(c) || c == '_';
}

bool RegHd6309Utils::regCharCaseEqual(char c, char regChar) {
    return c == regChar || (isalpha(c) && toupper(c) == regChar);
}

char RegHd6309Utils::regName1stChar(const RegName regName) {
    return toupper(char(regName));
}

char RegHd6309Utils::regName2ndChar(const RegName regName) {
    if (regName == REG_PC) return 'C';
    if (regName == REG_CC) return 'C';
    if (regName == REG_DP) return 'P';
    return 0;
}

bool RegHd6309Utils::compareRegName(const char *line, RegName regName) {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    return !isidchar(*line);
}

host::uint_t RegHd6309Utils::regNameLen(RegName regName) {
    return regName2ndChar(regName) == 0 ? 1 : 2;
}

char *RegHd6309Utils::outRegName(char *out, const RegName regName) {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) *out++ = r2;
    *out = 0;
    return out;
}

char *RegHd6309Utils::outCCRBits(char *out, uint8_t val) {
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

host::int_t RegHd6309Utils::encodeRegNumber(
    RegName regName, const RegName *table, const RegName *end) {
    for (const RegName *p = table; p < end; p++) {
        if (pgm_read_byte(p) == regName) return p - table;
    }
    return -1;
}

RegName RegHd6309Utils::parseRegName(
    const char *line, const RegName *table, const RegName *end) {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return REG_UNDEF;
}

RegName RegHd6309Utils::decodeRegNumber(
    host::uint_t regNum, const RegName *table, const RegName *end) {
    const RegName *entry = &table[regNum];
    return entry < end ? RegName(pgm_read_byte(entry)) : REG_UNDEF;
}

RegName RegHd6309Utils::getStackReg(host::uint_t bit, target::insn_t insnCode) {
    const RegName *table = (insnCode & 2) == 0
        ? &STACK_S_REGS[0] : &STACK_U_REGS[0];
    return RegName(pgm_read_byte(&table[bit]));
}

RegName RegHd6309Utils::parseBitOpReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(BIT_OP_REGS));
}

RegName RegHd6309Utils::parseTfmBaseReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(TFM_BASE_REGS));
}

host::int_t RegHd6309Utils::encodeBitOpReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(BIT_OP_REGS));
}

host::int_t RegHd6309Utils::encodeTfmBaseReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(TFM_BASE_REGS));
}

RegName RegHd6309Utils::decodeBitOpReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(BIT_OP_REGS));
}

RegName RegHd6309Utils::decodeTfmBaseReg(uint8_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(TFM_BASE_REGS));
}

char RegHd6309Utils::tfmSrcModeChar(host::uint_t mode) {
    return pgm_read_byte(&TFM_SRC_MODES[mode]);
}

char RegHd6309Utils::tfmDstModeChar(host::uint_t mode) {
    return pgm_read_byte(&TFM_DST_MODES[mode]);
}

static constexpr RegName MC6809_INDEX_REGS[] PROGMEM = {
    REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_B, REG_A, REG_UNDEF,
    REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_D
};
template<>
const RegName *IndexRegHd6309<MC6809>::begin() const {
    return ARRAY_BEGIN(MC6809_INDEX_REGS);
}
template<>
const RegName *IndexRegHd6309<MC6809>::end() const {
    return ARRAY_END(MC6809_INDEX_REGS);
}

static constexpr RegName HD6309_INDEX_REGS[] PROGMEM = {
    REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_B, REG_A, REG_E,
    REG_UNDEF, REG_UNDEF, REG_F, REG_D, REG_UNDEF, REG_UNDEF, REG_W
};
template<>
const RegName *IndexRegHd6309<HD6309>::begin() const {
    return ARRAY_BEGIN(HD6309_INDEX_REGS);
}
template<>
const RegName *IndexRegHd6309<HD6309>::end() const {
    return ARRAY_END(HD6309_INDEX_REGS);
}

static constexpr RegName MC6809_BASE_REGS[] PROGMEM = {
    REG_X, REG_Y, REG_U, REG_S
};
template<>
const RegName *BaseRegHd6309<MC6809>::begin() const {
    return ARRAY_BEGIN(MC6809_BASE_REGS);
}
template<>
const RegName *BaseRegHd6309<MC6809>::end() const {
    return ARRAY_END(MC6809_BASE_REGS);
}

static constexpr RegName HD6309_BASE_REGS[] PROGMEM = {
    REG_X, REG_Y, REG_U, REG_S, REG_W
};
template<>
const RegName *BaseRegHd6309<HD6309>::begin() const {
    return ARRAY_BEGIN(HD6309_BASE_REGS);
}
template<>
const RegName *BaseRegHd6309<HD6309>::end() const {
    return ARRAY_END(HD6309_BASE_REGS);
}

static constexpr RegName MC6809_DATA_REGS[] PROGMEM = {
    REG_D, REG_X, REG_Y, REG_U, REG_S, REG_PC, REG_UNDEF, REG_UNDEF,
    REG_A, REG_B, REG_CC, REG_DP
};
template<>
const RegName *DataRegHd6309<MC6809>::begin() const {
    return ARRAY_BEGIN(MC6809_DATA_REGS);
}
template<>
const RegName *DataRegHd6309<MC6809>::end() const {
    return ARRAY_END(MC6809_DATA_REGS);
}

static constexpr RegName HD6309_DATA_REGS[] PROGMEM = {
    REG_D, REG_X, REG_Y, REG_U, REG_S, REG_PC, REG_W, REG_V,
    REG_A, REG_B, REG_CC, REG_DP, REG_ZERO, REG_ZERO, REG_E, REG_F,
};
template<>
const RegName *DataRegHd6309<HD6309>::begin() const {
    return ARRAY_BEGIN(HD6309_DATA_REGS);
}
template<>
const RegName *DataRegHd6309<HD6309>::end() const {
    return ARRAY_END(HD6309_DATA_REGS);
}
