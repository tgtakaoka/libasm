#include <ctype.h>

#include "asm_i8080.h"

static const RegName TABLE_POINTERS[] = { B, D, H, SP };
static constexpr host::uint_t LENGTH_POINTERS =
    sizeof(TABLE_POINTERS) / sizeof(TABLE_POINTERS[0]);
static const RegName TABLE_STACK_REGS[] = { B, D, H, PSW };
static constexpr host::uint_t LENGTH_STACK_REGS =
    sizeof(TABLE_STACK_REGS) / sizeof(TABLE_STACK_REGS[0]);
static const RegName TABLE_INDEX_REGS[] = { B, D };
static constexpr host::uint_t LENGTH_INDEX_REGS =
    sizeof(TABLE_INDEX_REGS) / sizeof(TABLE_INDEX_REGS[0]);
static const RegName TABLE_DATA_REGS[] = { B, C, D, E, H, L, M, A };
static constexpr host::uint_t LENGTH_DATA_REGS =
    sizeof(TABLE_DATA_REGS) / sizeof(TABLE_DATA_REGS[0]);

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

static host::uint_t regNameLen(RegName regName) {
    if (regName3rdChar(regName)) return 3;
    return regName2ndChar(regName) == 0 ? 1 : 2;
}

static bool regCharCaseEqual(char c, char regChar) {
    return c == regChar || (isalpha(c) && toupper(c) == regChar);
}

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool compareRegName(const char *line, RegName regName) {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    const char r3 = regName3rdChar(regName);
    if (r3 && !regCharCaseEqual(*line++, r3)) return false;
    return !isidchar(*line);
}

static Error getInt16(const char *&in, target::uint16_t &val) {
    val = 0;
    const char *p;

    for (p = in; isxdigit(*p); p++)
        ;
    if (p > in && toupper(*p) == 'H') {
        for (p = in; isxdigit(*p); p++) {
            val <<= 4;
            val += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        }
        in = ++p;
        return OK;
    }

    for (p = in; *p >= '0' && *p < '8'; p++)
        ;
    if (p > in && toupper(*p) == 'O') {
        for (p = in; *p >= '0' && *p < '8'; p++) {
            val <<= 3;
            val += *p - '0';
        }
        in = ++p;
        return OK;
    }

    for (p = in; *p == '0' || *p == '1'; p++)
        ;
    if (p > in && toupper(*p) == 'B') {
        for (p = in; *p == '0' || *p == '1'; p++) {
            val <<= 1;
            val += *p - '0';
        }
        in = ++p;
        return OK;
    }

    p = in;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    while (isdigit(*p)) {
        val *= 10;
        val += *p++ - '0';
    }
    in = p;
    if (sign == '-') val = -(target::int16_t)val;
    return OK;
}

Error AsmI8080::getOperand16(const char *&in, target::uint16_t &val) {
    if (getInt16(in, val) == OK) return setError(OK);
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isidchar(in[idx]); idx++) {
        symbol_buffer[idx] = in[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        in += idx;
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

static RegName parseRegName(
    const char *line, const RegName *table, const RegName *end) {
    for (const RegName *entry = table; entry < end; entry++) {
        if (compareRegName(line, *entry)) return *entry;
    }
    return NONE;
}

static host::int_t encodeRegNumber(
    const RegName regName, const RegName *table, const RegName *end) {
    for (host::int_t num = 0; table < end; num++, table++) {
        if (regName == *table) return num;
    }
    return -1;
}

const char *AsmI8080::encodePointerReg(
    const char *line, Insn &insn, const RegName *table, const RegName *end) {
    const RegName regName = parseRegName(line, table, end);
    const host::int_t num = encodeRegNumber(regName, table, end);
    if (num < 0) {
        setError(UNKNOWN_REGISTER);
    } else {
        insn.setInsnCode(insn.insnCode() | (num << 4));
        line += regNameLen(regName);
        setError(OK);
    }
    return line;
}

const char *AsmI8080::encodeDataReg(const char *line, Insn &insn) {
    const RegName regName = parseRegName(
        line, &TABLE_DATA_REGS[0], &TABLE_DATA_REGS[LENGTH_DATA_REGS]);
    const host::int_t num = encodeRegNumber(
        regName, &TABLE_DATA_REGS[0], &TABLE_DATA_REGS[LENGTH_DATA_REGS]);
    if (num < 0) {
        setError(UNKNOWN_REGISTER);
    } else {
        if (insn.insnFormat() == DATA_REG)
            insn.setInsnCode(insn.insnCode() | (num << 3));
        if (insn.insnFormat() == LOW_DATA_REG)
            insn.setInsnCode(insn.insnCode() | num);
        line += regNameLen(regName);
        setError(OK);
    }
    return line;
}

const char *AsmI8080::encodeDataDataReg(const char *line, Insn &insn) {
    const RegName dstReg = parseRegName(
        line, &TABLE_DATA_REGS[0], &TABLE_DATA_REGS[LENGTH_DATA_REGS]);
    if (dstReg == NONE) {
        setError(UNKNOWN_REGISTER);
        return line;
    }
    line += regNameLen(dstReg);
    if (*line++ != ',') {
        setError(UNKNOWN_OPERAND);
        return --line;
    }
    const RegName srcReg = parseRegName(
        line, &TABLE_DATA_REGS[0], &TABLE_DATA_REGS[LENGTH_DATA_REGS]);
    if (srcReg == NONE) {
        setError(UNKNOWN_REGISTER);
        return line;
    }
    line += regNameLen(srcReg);

    const host::uint_t dstNum = encodeRegNumber(
        dstReg, &TABLE_DATA_REGS[0], &TABLE_DATA_REGS[LENGTH_DATA_REGS]);
    const host::uint_t srcNum = encodeRegNumber(
        srcReg, &TABLE_DATA_REGS[0], &TABLE_DATA_REGS[LENGTH_DATA_REGS]);
    insn.setInsnCode(insn.insnCode() | (dstNum << 3) | srcNum);

    setError(OK);
    return line;
}

const char *AsmI8080::encodeVectorNo(const char *line, Insn &insn) {
    target::uint16_t vecNo;
    if (getOperand16(line, vecNo) == OK) {
        if (vecNo < 8) {
            insn.setInsnCode(insn.insnCode() | (vecNo << 3));
            setError(OK);
        } else {
            setError(UNKNOWN_OPERAND);
        }
    }
    return line;
}

Error AsmI8080::encodeImmediate(const char *line, Insn &insn) {
    if (insn.insnFormat() != NO_FORMAT && *line++ != ',')
        return setError(UNKNOWN_OPERAND);
    target::uint16_t val;
    if (getOperand16(line, val)) return getError();
    if (insn.addrMode() == IMMEDIATE_8)
        insn.emitByte(val);
    if (insn.addrMode() == IMMEDIATE_16)
        insn.emitUint16(val);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

Error AsmI8080::encodeDirect(const char *line, Insn &insn) {
    target::uint16_t addr;
    if (getOperand16(line, addr)) return getError();
    insn.emitUint16(addr);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

Error AsmI8080::encodeIoaddr(const char *line, Insn &insn) {
    target::uint16_t addr;
    if (getOperand16(line, addr)) return getError();
    insn.emitByte(addr);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

Error AsmI8080::encode(
    const char *line, Insn &insn, target::uintptr_t addr, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(addr);
    line = skipSpace(line);
    if (!*line) return setError(NO_TEXT);
    const char *endName;
    for (endName = line; isidchar(*endName); endName++)
        ;
    insn.setName(line, endName);
    line = skipSpace(endName);

    if (TableI8080.search(insn, insn.name()))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.insnFormat()) {
    case INHERENT:
        setError(OK);
        break;
    case POINTER_REG:
        line = encodePointerReg(
            line, insn,
            &TABLE_POINTERS[0], &TABLE_POINTERS[LENGTH_POINTERS]);
        break;
    case STACK_REG:
        line = encodePointerReg(
            line, insn,
            &TABLE_STACK_REGS[0], &TABLE_STACK_REGS[LENGTH_STACK_REGS]);
        break;
    case INDEX_REG:
        line = encodePointerReg(
            line, insn,
            &TABLE_INDEX_REGS[0], &TABLE_INDEX_REGS[LENGTH_INDEX_REGS]);
        break;
    case DATA_REG:
    case LOW_DATA_REG:
        line = encodeDataReg(line, insn);
        break;
    case DATA_DATA_REG:
        line = encodeDataDataReg(line, insn);
        break;
    case VECTOR_NO:
        line = encodeVectorNo(line, insn);
        break;
    default:
        break;
    }
    if (getError()) return getError();

    insn.emitByte(insn.insnCode());
    switch (insn.addrMode()) {
    case INHERENT:
        return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
    case IMMEDIATE_8:
    case IMMEDIATE_16:
        return encodeImmediate(line, insn);
    case DIRECT:
        return encodeDirect(line, insn);
    case IOADDR:
        return encodeIoaddr(line, insn);
    default:
        return setError(UNKNOWN_OPERAND);
    }
}
