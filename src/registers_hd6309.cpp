#include <ctype.h>

#include "config_hd6309.h"

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

bool RegistersUtils::isidchar(const char c) {
    return isalnum(c) || c == '_';
}

bool RegistersUtils::regCharCaseEqual(char c, char regChar) {
    return c == regChar || (isalpha(c) && toupper(c) == regChar);
}

char RegistersUtils::regName1stChar(const RegName regName) {
    return (regName == REG_DP) ? 'D' : char(regName);
}

char RegistersUtils::regName2ndChar(const RegName regName) {
    if (regName == REG_PC) return 'C';
    if (regName == REG_CC) return 'C';
    if (regName == REG_DP) return 'P';
    return 0;
}

bool RegistersUtils::compareRegName(const char *line, RegName regName) {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    return !isidchar(*line);
}

host::uint_t RegistersUtils::regNameLen(RegName regName) {
    return regName2ndChar(regName) == 0 ? 1 : 2;
}

char *RegistersUtils::outRegName(char *out, const RegName regName) {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) *out++ = r2;
    *out = 0;
    return out;
}

char *RegistersUtils::outCCRBits(char *out, target::byte_t val) {
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

host::int_t RegistersUtils::encodeRegNumber(
    RegName regName, const RegName *table, const RegName *end) {
    for (const RegName *p = table; p < end; p++) {
        if (pgm_read_byte(p) == regName) return p - table;
    }
    return -1;
}

RegName RegistersUtils::parseRegName(
    const char *line, const RegName *table, const RegName *end) {
    for (const RegName *p = table; p < end; p++) {
        const RegName regName = RegName(pgm_read_byte(p));
        if (compareRegName(line, regName)) return regName;
    }
    return REG_UNDEF;
}

RegName RegistersUtils::decodeRegNumber(
    host::uint_t regNum, const RegName *table, const RegName *end) {
    const RegName *entry = &table[regNum];
    return entry < end ? RegName(pgm_read_byte(entry)) : REG_UNDEF;
}

RegName RegistersUtils::getStackReg(host::uint_t bit, target::insn_t insnCode) {
    const RegName *table = (insnCode & 2) == 0
        ? &STACK_S_REGS[0] : &STACK_U_REGS[0];
    return RegName(pgm_read_byte(&table[bit]));
}

RegName RegistersUtils::parseBitOpReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(BIT_OP_REGS));
}

RegName RegistersUtils::parseTfmBaseReg(const char *line) {
    return parseRegName(line, ARRAY_RANGE(TFM_BASE_REGS));
}

host::int_t RegistersUtils::encodeBitOpReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(BIT_OP_REGS));
}

host::int_t RegistersUtils::encodeTfmBaseReg(RegName regName) {
    return encodeRegNumber(regName, ARRAY_RANGE(TFM_BASE_REGS));
}

RegName RegistersUtils::decodeBitOpReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(BIT_OP_REGS));
}

RegName RegistersUtils::decodeTfmBaseReg(target::byte_t regNum) {
    return decodeRegNumber(regNum, ARRAY_RANGE(TFM_BASE_REGS));
}

char RegistersUtils::tfmSrcModeChar(host::uint_t mode) {
    return pgm_read_byte(&TFM_SRC_MODES[mode]);
}

char RegistersUtils::tfmDstModeChar(host::uint_t mode) {
    return pgm_read_byte(&TFM_DST_MODES[mode]);
}

static constexpr RegName MC6809_INDEX_REGS[] PROGMEM = {
    REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_B, REG_A, REG_UNDEF,
    REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_D
};
template<>
const RegName *IndexRegisters<MC6809>::begin() const {
    return ARRAY_BEGIN(MC6809_INDEX_REGS);
}
template<>
const RegName *IndexRegisters<MC6809>::end() const {
    return ARRAY_END(MC6809_INDEX_REGS);
}

static constexpr RegName HD6309_INDEX_REGS[] PROGMEM = {
    REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_UNDEF, REG_B, REG_A, REG_E,
    REG_UNDEF, REG_UNDEF, REG_F, REG_D, REG_UNDEF, REG_UNDEF, REG_W
};
template<>
const RegName *IndexRegisters<HD6309>::begin() const {
    return ARRAY_BEGIN(HD6309_INDEX_REGS);
}
template<>
const RegName *IndexRegisters<HD6309>::end() const {
    return ARRAY_END(HD6309_INDEX_REGS);
}

static constexpr RegName MC6809_BASE_REGS[] PROGMEM = {
    REG_X, REG_Y, REG_U, REG_S
};
template<>
const RegName *BaseRegisters<MC6809>::begin() const {
    return ARRAY_BEGIN(MC6809_BASE_REGS);
}
template<>
const RegName *BaseRegisters<MC6809>::end() const {
    return ARRAY_END(MC6809_BASE_REGS);
}

static constexpr RegName HD6309_BASE_REGS[] PROGMEM = {
    REG_X, REG_Y, REG_U, REG_S, REG_W
};
template<>
const RegName *BaseRegisters<HD6309>::begin() const {
    return ARRAY_BEGIN(HD6309_BASE_REGS);
}
template<>
const RegName *BaseRegisters<HD6309>::end() const {
    return ARRAY_END(HD6309_BASE_REGS);
}

static constexpr RegName MC6809_DATA_REGS[] PROGMEM = {
    REG_D, REG_X, REG_Y, REG_U, REG_S, REG_PC, REG_UNDEF, REG_UNDEF,
    REG_A, REG_B, REG_CC, REG_DP
};
template<>
const RegName *DataRegisters<MC6809>::begin() const {
    return ARRAY_BEGIN(MC6809_DATA_REGS);
}
template<>
const RegName *DataRegisters<MC6809>::end() const {
    return ARRAY_END(MC6809_DATA_REGS);
}

static constexpr RegName HD6309_DATA_REGS[] PROGMEM = {
    REG_D, REG_X, REG_Y, REG_U, REG_S, REG_PC, REG_W, REG_V,
    REG_A, REG_B, REG_CC, REG_DP, REG_ZERO, REG_ZERO, REG_E, REG_F,
};
template<>
const RegName *DataRegisters<HD6309>::begin() const {
    return ARRAY_BEGIN(HD6309_DATA_REGS);
}
template<>
const RegName *DataRegisters<HD6309>::end() const {
    return ARRAY_END(HD6309_DATA_REGS);
}
