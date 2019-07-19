#include <cctype>

#include "asm_mc6809.h"
#include "string_utils.h"

TableMc6809 AsmMc6809::_tableMc6809;

static char regName1stChar(const RegName regName) {
    return (regName == DP) ? 'D' : char(regName);
}

static char regName2ndChar(const RegName regName) {
    if (regName == PC) return 'C';
    if (regName == CC) return 'C';
    if (regName == DP) return 'P';
    return 0;
}

static const RegName tableBaseReg[4] = { X, Y, U, S };
static const RegName tableIndexReg[16] = {
    NONE, NONE, NONE, NONE, NONE, B, A, NONE, NONE, NONE, NONE, D, NONE, NONE, NONE, NONE,
};
static const char tableCCBitName[8] = { 'E', 'F', 'H', 'I', 'N', 'Z', 'V', 'C' };
static const RegName tableStackS[8] = { CC, A, B, DP, X, Y, U, PC };
static const RegName tableStackU[8] = { CC, A, B, DP, X, Y, S, PC };
static const RegName tableRegNum[16] = {
    D, X, Y, U, S, PC, NONE, NONE, A, B, CC, DP, NONE, NONE, NONE, NONE,
};

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

Error AsmMc6809::getOperand16(const char *&in, target::uint16_t &val) {
    if (getInt16(in, val)) return OK;
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
    return UNKNOWN_OPERAND;
}

host::uint_t AsmMc6809::regNameLen(RegName regName) {
    return regName2ndChar(regName) == 0 ? 1 : 2;
}

static bool regCharCaseEqual(char c, char regChar) {
    return c == regChar || (isalpha(c) && toupper(c) == regChar);
}

bool AsmMc6809::compareRegName(const char *line, RegName regName) {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    const char r2 = regName2ndChar(regName);
    if (r2 && !regCharCaseEqual(*line++, r2)) return false;
    return !isidchar(*line);
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

Error AsmMc6809::encodeStackOp(const char *line, Insn &insn) {
    const RegName *table = ((insn.insnCode() & 2) == 0 ? &tableStackS[0] : &tableStackU[0]);
    target::byte_t post = 0;
    while (*line) {
        host::uint_t bit = 0;
        for (host::uint_t index = 0, mask = 0x01; index < 8; index++, mask <<= 1) {
            const RegName regName = table[index];
            if (compareRegName(line, regName)) {
                line += regNameLen(regName);
                bit = mask;
                break;
            }
        }
        if (bit == 0) return setError(UNKNOWN_REGISTER);
        post |= bit;
        if (*line == ',') line++;
    }
    emitInsnCode(insn);
    emitByte(insn, post);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

Error AsmMc6809::encodeRegisters(const char *line, Insn &insn) {
    RegName regName;
    constexpr host::uint_t tableLen = sizeof(tableRegNum) / sizeof(tableRegNum[0]);
    if ((regName = parseRegName(line, tableRegNum, tableLen)) == NONE)
        return setError(UNKNOWN_REGISTER);
    line += regNameLen(regName);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    target::byte_t post = encodeRegNumber(regName, tableRegNum, tableLen) << 4;
    if ((regName = parseRegName(line, tableRegNum, tableLen)) == NONE)
        return setError(UNKNOWN_REGISTER);
    line += regNameLen(regName);
    if (*skipSpace(line)) return setError(GARBAGE_AT_END);
    post |= encodeRegNumber(regName, tableRegNum, tableLen);
    emitInsnCode(insn);
    emitByte(insn, post);
    return setError(OK);
}

Error AsmMc6809::encodeRelative(const char *line, Insn &insn) {
    target::uintptr_t addr;
    if (getOperand16(line, addr)) return setError(UNKNOWN_OPERAND);
    const target::opcode_t prefix = _tableMc6809.prefixCode(insn.insnCode());
    const host::uint_t insnLen = (_tableMc6809.isPrefixCode(prefix) ? 2 : 1) + insn.oprLen();
    const target::uintptr_t base = insn.address() + insnLen;
    const target::ptrdiff_t delta = addr - base;
    emitInsnCode(insn);
    if (insn.oprLen() == 1) {
        if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
        emitByte(insn, target::byte_t(delta));
    } else {
        emitUint16(insn, delta);
    }
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

Error AsmMc6809::encodeImmediate(const char *line, Insn &insn) {
    if (*line++ != '#') return setError(UNKNOWN_OPERAND);
    emitInsnCode(insn);
    if (insn.oprLen() == 1 || insn.oprLen() == 2) {
        target::uint16_t val;
        if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
        if (insn.oprLen() == 1) emitByte(insn, target::byte_t(val));
        else emitUint16(insn, val);
    }
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);}

Error AsmMc6809::encodeDirect(const char *line, Insn &insn) {
    if (*line == '<') line++;
    emitInsnCode(insn);
    target::uintptr_t dir;
    if (getOperand16(line, dir)) return setError(UNKNOWN_OPERAND);
    emitByte(insn, target::byte_t(dir));
    return setError(OK);
}

Error AsmMc6809::encodeExtended(const char *line, Insn &insn) {
    if (*line == '>') line++;
    emitInsnCode(insn);
    target::uintptr_t addr;
    if (getOperand16(line, addr)) return setError(UNKNOWN_OPERAND);
    emitUint16(insn, addr);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

host::int_t AsmMc6809::encodeRegNumber(RegName regName, const RegName *table, host::uint_t len) {
    for (host::uint_t idx = 0; idx < len; idx++) {
        if (table[idx] == regName) return idx;
    }
    return -1;
}

RegName AsmMc6809::parseRegName(const char *line, const RegName *table, host::uint_t len) {
    for (host::uint_t idx = 0; idx < len; idx++) {
        if (compareRegName(line, table[idx])) return table[idx];
    }
    return NONE;
}

Error AsmMc6809::encodeIndexed(const char *line, Insn &insn) {
    emitInsnCode(insn);
    const bool indir = (*line == '[');
    RegName base = NONE;
    RegName index = NONE;
    host::int_t incr = 0;                    // auto decrement/increment
    target::uintptr_t addr;
    constexpr host::uint_t baseLen = sizeof(tableBaseReg) / sizeof(tableBaseReg[0]);
    constexpr host::uint_t indexLen = sizeof(tableIndexReg) / sizeof(tableIndexReg[0]);
    if (indir) line++;
    if (*line != ',') {
        if ((index = parseRegName(line, tableIndexReg, indexLen)) != NONE) {
            line += regNameLen(index);  // index register
        } else {
            if (getOperand16(line, addr)) return setError(UNKNOWN_OPERAND);
            index = OFFSET;             // index is in addr
        }
    }
    if (*line == ',') {
        line++;
        if (index == NONE) {
            while (*line == '-') {
                line++;
                incr--;
            }
        }
        if ((base = parseRegName(line, tableBaseReg, baseLen)) != NONE || compareRegName(line, PC)) {
            if (base == NONE) base = PC;
            line += regNameLen(base);
        } else setError(UNKNOWN_OPERAND);
        if (index == NONE && incr == 0) {
            while (*line == '+') {
                line++;
                incr++;
            }
        }
    }
    if (indir && *line++ != ']') return setError(UNKNOWN_OPERAND);
    if (*skipSpace(line)) return setError(GARBAGE_AT_END);

    target::byte_t post;
    if (base == NONE) {                 // [n16]
        if (index != OFFSET) return setError(UNKNOWN_OPERAND);
        emitByte(insn, 0x9F);
        emitUint16(insn, addr);
        return setError(OK);
    }
    if (base == PC) {                   // n,PC [n,PC]
        if (index != OFFSET || incr != 0) return setError(UNKNOWN_OPERAND);
        post = indir ? 0x10 : 0;
        target::ptrdiff_t delta = addr - (insn.address() + insn.insnLen() + 2);
        if (delta >= -128 && delta < 128) {
            emitByte(insn, 0x8C | post);
            emitByte(insn, (uint8_t)delta);
            return setError(OK);
        }
        delta = addr - (insn.address() + insn.insnLen() + 3);
        emitByte(insn, 0x8D | post);
        emitUint16(insn, delta);
        return setError(OK);
    }
    post = encodeRegNumber(base, tableBaseReg, baseLen) << 5;
    if (indir) post |= 0x10;
    if (index == NONE) {              // ,R [,R] ,R+ ,R- ,R++ ,--R [,R++] [,--R]
        if (incr == 0) post |= 0x84;
        else if (incr == 2) post |= 0x81;
        else if (incr == -2) post |= 0x83;
        else if (!indir && incr == 1) post |= 0x80;
        else if (!indir && incr == -1) post |= 0x82;
        else return setError(UNKNOWN_OPERAND);
        emitByte(insn, post);
        return setError(OK);
    }
    if (index != OFFSET) {              // R,R
        post |= encodeRegNumber(index, tableIndexReg, indexLen);
        emitByte(insn, 0x80 | post);
        return setError(OK);
    }
    const target::intptr_t offset = addr;
    if (offset >= -16 && offset < 16 && !indir) { // n5.R
        post |= (offset & 0x1F);
        emitByte(insn, post);
        return setError(OK);
    }
    if (offset >= -128 && offset < 128) { // n8,R [n8,R]
        post |= 0x88;
        emitByte(insn, post);
        emitByte(insn, (uint8_t)offset);
        return setError(OK);
    }
    // n16,R [n16,R]
    post |= 0x89;
    emitByte(insn, post);
    emitUint16(insn, offset);
    return setError(OK);
}

Error AsmMc6809::determineAddrMode(const char *line, Insn &insn) {
    switch (*line) {
    case '#': setAddrMode(insn, IMMEDIATE); break;
    case '<': setAddrMode(insn, DIRECT_PG); break;
    case '>': setAddrMode(insn, EXTENDED); break;
    case '[':
    case ',': setAddrMode(insn, INDEXED); break;
    default:
        constexpr host::uint_t indexLen = sizeof(tableIndexReg) / sizeof(tableIndexReg[0]);
        RegName index;
        if ((index = parseRegName(line, tableIndexReg, indexLen)) != NONE) {
            line += regNameLen(index);
            if (*line == ',') {
                setAddrMode(insn, INDEXED);
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
        target::uint16_t val;
        if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
        if (*line == ',') setAddrMode(insn, INDEXED);
        else setAddrMode(insn, val < 0x100 ? DIRECT_PG : EXTENDED);
        break;
    }
    return OK;
}

Error AsmMc6809::encode(
    const char *line, Insn &insn, target::uintptr_t addr, SymbolTable *symtab) {
    reset(symtab);
    resetAddress(insn, addr);
    line = skipSpace(line);
    if (!*line) return setError(NO_TEXT);
    const char *endName;
    for (endName = line; isidchar(*endName); endName++)
        ;
    setName(insn, line, endName);
    line = skipSpace(endName);

    if (_tableMc6809.search(insn, insn.name()))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHERENT:
        emitInsnCode(insn);
        return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
    case RELATIVE:
        return encodeRelative(line, insn);
    case STACK_OP:
        return encodeStackOp(line, insn);
    case REGISTERS:
        return encodeRegisters(line, insn);
    default:
        break;
    }

    if (determineAddrMode(line, insn)) return getError();
    if (_tableMc6809.search(insn, insn.addrMode()))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case IMMEDIATE: return encodeImmediate(line, insn);
    case DIRECT_PG: return encodeDirect(line, insn);
    case EXTENDED: return encodeExtended(line, insn);
    case INDEXED: return encodeIndexed(line, insn);
    default: return setError(UNKNOWN_OPERAND);
    }
}
