#include <cctype>

#include "asm_hd6309.h"
#include "string_utils.h"

static const RegName TABLE_REGISTERS[] = {
    D, X, Y, U, S, PC, W, V, A, B, CC, DP, ZERO, ZERO, E, F,
};
constexpr host::uindex_t LENGTH_REGISTERS =
    sizeof(TABLE_REGISTERS) / sizeof(TABLE_REGISTERS[0]);

static const RegName TABLE_INDEX_REGS[] = {
    NONE, NONE, NONE, NONE, NONE, B, A, E, NONE, NONE, F, D, NONE, NONE, W
};
constexpr host::uindex_t LENGTH_INDEXES =
    sizeof(TABLE_INDEX_REGS) / sizeof(TABLE_INDEX_REGS[0]);
static const RegName TABLE_BASE_REGS[] = { X, Y, U, S, W };
constexpr host::uindex_t LENGTH_BASES =
    sizeof(TABLE_BASE_REGS) / sizeof(TABLE_BASE_REGS[0]);

static const RegName TABLE_TFM_REGS[] = { D, X, Y, U, S };
constexpr host::uindex_t LENGTH_TFM_REGS =
    sizeof(TABLE_TFM_REGS) / sizeof(TABLE_TFM_REGS[0]);
static const char TABLE_TFM_SRC_MODES[4] = { '+', '-', '+',   0 };
static const char TABLE_TFM_DST_MODES[4] = { '+', '-',   0, '+' };

host::int_t AsmHd6309::encodeRegister(RegName regName) const {
    return encodeRegNumber(regName, &TABLE_REGISTERS[0],
                           &TABLE_REGISTERS[LENGTH_REGISTERS]);
}

RegName AsmHd6309::parseRegister(const char *line) const {
    return parseRegName(line, &TABLE_REGISTERS[0],
                        &TABLE_REGISTERS[LENGTH_REGISTERS]);
}

host::int_t AsmHd6309::encodeIndexReg(RegName regName) const {
    return encodeRegNumber(regName, &TABLE_INDEX_REGS[0],
                           &TABLE_INDEX_REGS[LENGTH_INDEXES]);
}

RegName AsmHd6309::parseIndexReg(const char *line) const {
    return parseRegName(line, &TABLE_INDEX_REGS[0],
                        &TABLE_INDEX_REGS[LENGTH_INDEXES]);
}

host::int_t AsmHd6309::encodeBaseReg(RegName regName) const {
    return encodeRegNumber(regName, &TABLE_BASE_REGS[0],
                           &TABLE_BASE_REGS[LENGTH_BASES]);
}

RegName AsmHd6309::parseBaseReg(const char *line) const {
    return parseRegName(line, &TABLE_BASE_REGS[0],
                        &TABLE_BASE_REGS[LENGTH_BASES]);
}

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

Error AsmHd6309::getOperand32(const char *&in, target::uint32_t &val) const {
    if (getUint32(in, val)) return OK;
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

Error AsmHd6309::encodeImmediateExtra(const char *line, Insn &insn) {
    if (insn.oprLen() == 4) {
        uint32_t val;
        Error error = getOperand32(line, val);
        if (error == OK) {
            emitUint32(insn, val);
            if (*skipSpace(line)) error = GARBAGE_AT_END;
        }
        setError(error);
        return OK;                      // processed
    }
    return UNKNOWN_OPERAND;             // bypassed
}

Error AsmHd6309::encodeIndexedExtra(
    Insn &insn, RegName index, target::uintptr_t addr,
    RegName base, host::int_t incr, bool indir) {
    if (base != W) return UNKNOWN_OPERAND; // bypass

    if (index != NONE && index != OFFSET) {
        return setError(UNKNOWN_OPERAND);
        return OK;                      // processed/error
    }
    target::byte_t post;
    if (index == OFFSET) post = 0xAF;   // n16,W [n16,W]
    else if (incr == 0) post = 0x8F;    // ,W [,W]
    else if (incr == 2) post = 0xCF;    // ,W++ [,W++]
    else if (incr == -2) post = 0xEF;   // ,--W [,--W]
    else {
        setError(UNKNOWN_OPERAND);
        return OK;                      // processed/error
    }
    if (indir) post++;
    emitByte(insn, post);
    if (index == OFFSET) emitUint16(insn, addr);
    return setError(OK);                // processed/ok
}

Error AsmHd6309::encodeBitOperation(const char *line, Insn &insn) {
    uint8_t post = 0;
    if (compareRegName(line, CC)) {
        line += 2;
    } else if (compareRegName(line, A)) {
        post |= 0x40;
        line += 1;
    } else if (compareRegName(line, B)) {
        post |= 0x80;
        line += 1;
    } else {
        return setError(UNKNOWN_REGISTER);
    }
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    uint16_t pos;
    if (getOperand16(line, pos)) return setError(UNKNOWN_OPERAND);
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    post |= (pos << 3);
    if (getOperand16(line, pos)) return setError(UNKNOWN_OPERAND);
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    post |= pos;
    emitInsnCode(insn);
    emitByte(insn, post);
    return encodeDirect(line, insn, /* emitInsn */ false);
}

Error AsmHd6309::encodeImmediatePlus(const char *line, Insn &insn) {
    if (*line++ != '#') return setError(UNKNOWN_OPERAND);
    uint16_t val;
    if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);

    if (determineAddrMode(line, insn)) return getError();
    switch (insn.addrMode()) {
    case DIRECT_PG: setAddrMode(insn, IMM_DIRECT); break;
    case EXTENDED: setAddrMode(insn, IMM_EXTENDED); break;
    case INDEXED: setAddrMode(insn, IMM_INDEXED); break;
    default: return setError(UNKNOWN_OPERAND);
    }
    if (TableHd6309.search(insn, insn.addrMode()))
        return setError(UNKNOWN_INSTRUCTION);
    emitInsnCode(insn);
    emitByte(insn, (uint8_t)val);
    switch (insn.addrMode()) {
    case IMM_DIRECT:
        return encodeDirect(line, insn, /* emitInsn */ false);
    case IMM_EXTENDED:
        return encodeExtended(line, insn, /* emitInsn */ false);
    case IMM_INDEXED:
        return encodeIndexed(line, insn, /* emitInsn */ false);
    default: return setError(UNKNOWN_OPERAND);
    }
}

Error AsmHd6309::encodeTransferMemory(const char *line, Insn &insn) {
    constexpr const RegName *END_TFM_REGS = &TABLE_TFM_REGS[LENGTH_TFM_REGS];
    RegName regName = parseRegName(line, &TABLE_TFM_REGS[0], END_TFM_REGS);
    if (regName == NONE) return setError(UNKNOWN_REGISTER);
    line += regNameLen(regName);
    char srcMode = 0;
    if (*line == '+' || *line == '-') srcMode = *line++;
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    target::opcode_t post =
        encodeRegNumber(regName, TABLE_TFM_REGS, END_TFM_REGS) << 4;

    regName = parseRegName(line, TABLE_TFM_REGS, END_TFM_REGS);
    if (regName == NONE) return setError(UNKNOWN_REGISTER);
    line += regNameLen(regName);
    char dstMode = 0;
    if (*line == '+' || *line == '-') dstMode = *line++;
    if (*skipSpace(line)) return setError(GARBAGE_AT_END);
    post |= encodeRegNumber(regName, TABLE_TFM_REGS, END_TFM_REGS);

    for (uint8_t mode = 0; mode < sizeof(TABLE_TFM_SRC_MODES); mode++) {
        if (srcMode == TABLE_TFM_SRC_MODES[mode] && dstMode == TABLE_TFM_DST_MODES[mode]) {
            target::opcode_t prefixCode = TableHd6309.prefixCode(insn.insnCode());
            setInsnCode(insn,  TableHd6309.insnCode(prefixCode, 0x38 + mode));
            emitInsnCode(insn);
            emitByte(insn, post);
            return setError(OK);
        }
    }
    return setError(UNKNOWN_OPERAND);
}

#include <stdio.h>
Error AsmHd6309::encode(const char *line, Insn &insn, target::uintptr_t addr, SymbolTable *symtab) {
    if (AsmMc6809::encode(line, insn, addr, symtab) == OK)
        return setError(OK);
    line = skipSpace(line);
    if (!*line) return setError(NO_TEXT);
    while (isidchar(*line)) line++;
    line = skipSpace(line);

    if (TableHd6309.search(insn, insn.name())) return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHERENT:
        emitInsnCode(insn);
        return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
    case REGISTERS:
        return encodeRegisters(line, insn);
    case IMM_DIRECT: case IMM_EXTENDED: case IMM_INDEXED:
        return encodeImmediatePlus(line, insn);
    case BIT_OPERATION:
        return encodeBitOperation(line, insn);
    case TRANSFER_MEM:
        return encodeTransferMemory(line, insn);
    default:
        break;
    }

    if (determineAddrMode(line, insn)) return getError();
    if (TableHd6309.search(insn, insn.addrMode()))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case IMMEDIATE:
        return encodeImmediate(line, insn);
    case DIRECT_PG:
        return encodeDirect(line, insn);
    case EXTENDED:
        return encodeExtended(line, insn);
    case INDEXED:
        return encodeIndexed(line, insn);
    default: return setError(UNKNOWN_OPERAND);
    }
}
