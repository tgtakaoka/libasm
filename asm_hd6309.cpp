#include <cctype>

#include "asm_hd6309.h"
#include "string_utils.h"

TableHd6309 AsmHd6309::_tableHd6309;

static const RegName tableBaseReg[4] = { X, Y, U, S };
static const RegName tableIndexReg[16] = {
    NONE, NONE, NONE, NONE, NONE, B, A, E, NONE, NONE, F, D, NONE, NONE, W, NONE,
};
static const RegName tableRegNum[16] = {
    D, X, Y, U, S, PC, W, V, A, B, CC, DP, ZERO, ZERO, E, F,
};

static const RegName tableTfmBaseReg[16] = {
    D, X, Y, U, S, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
};

static const char tableTfmSrcMode[4] = { '+', '-', '+',   0 };
static const char tableTfmDstMode[4] = { '+', '-',   0, '+' };

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

static const char *copyWord(const char *line, char *dst, host::int_t max_len) {
    for (host::int_t i = 0; *line && *line != ' ' && i < max_len; i++)
        *dst++ = *line++;
    *dst = 0;
    return skipSpace(line);
}

Error AsmHd6309::getOperand32(const char *&in, target::dword_t &val) {
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

Error AsmHd6309::encodeImmediate(const char *line) {
    if (*line++ != '#') return setError(UNKNOWN_OPERAND);
    addInsnCode();
    if (_oprLen == 1 || _oprLen == 2) {
        uint16_t val;
        if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
        if (_oprLen == 1) addByte((uint8_t)val);
        else addWord(val);
    }
//#ifdef SUPPORT_HD6309
    else if (_oprLen == 4) {
        uint32_t val;
        if (getOperand32(line, val)) return setError(UNKNOWN_OPERAND);
        addDword(val);
    }
//#endif
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

Error AsmHd6309::encodeIndexed(const char *line) {
//    E_HD6309(if (_mode == IDX)) addInsnCode();
    const bool indir = (*line == '[');
    RegName base = NONE;
    RegName index = NONE;
    int8_t incr = 0;                    // auto decrement/increment
    uint16_t addr;
    constexpr host::uint_t indexLen = sizeof(tableIndexReg) / sizeof(tableIndexReg[0]);
    constexpr host::uint_t baseLen = sizeof(tableBaseReg) / sizeof(tableBaseReg[0]);
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
//#ifdef SUPPORT_HD6309
        } else if (compareRegName(line, W)) {
            base = W;
            line += regNameLen(W);
//#endif
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

    uint8_t post;
    if (base == NONE) {                 // [n16]
        if (index != OFFSET) return setError(UNKNOWN_OPERAND);
        addByte(0x9F);
        addWord(addr);
        return setError(OK);
    }
//#ifdef SUPPORT_HD6309
    if (base == W) {
        if (index != NONE && index != OFFSET) return setError(UNKNOWN_OPERAND);
        if (index == OFFSET) post = 0xAF; // n16,W [n16,W]
        else if (incr == 0) post = 0x8F; // ,W [,W]
        else if (incr == 2) post = 0xCF; // ,W++ [,W++]
        else if (incr == -2) post = 0xEF; // ,--W [,--W]
        else return setError(UNKNOWN_OPERAND);
        if (indir) post++;
        addByte(post);
        if (index == OFFSET) addWord(addr);
        return setError(OK);
    }
//#endif
    if (base == PC) {                   // n,PC [n,PC]
        if (index != OFFSET || incr != 0) return setError(UNKNOWN_OPERAND);
        post = indir ? 0x10 : 0;
        int16_t delta = addr - (_address + _insnLen + 2);
        if (delta >= -128 && delta < 128) {
            addByte(0x8C | post);
            addByte((uint8_t)delta);
            return setError(OK);
        }
        delta = addr - (_address + _insnLen + 3);
        addByte(0x8D | post);
        addWord(delta);
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
        addByte(post);
        return setError(OK);
    }
    if (index != OFFSET) {              // R,R
        post |= encodeRegNumber(index, tableIndexReg, indexLen);
        addByte(0x80 | post);
        return setError(OK);
    }
    const int16_t offset = addr;
    if (offset >= -16 && offset < 16 && !indir) { // n5.R
        post |= (offset & 0x1F);
        addByte(post);
        return setError(OK);
    }
    if (offset >= -128 && offset < 128) { // n8,R [n8,R]
        post |= 0x88;
        addByte(post);
        addByte((uint8_t)offset);
        return setError(OK);
    }
    // n16,R [n16,R]
    post |= 0x89;
    addByte(post);
    addWord(offset);
    return setError(OK);
}

Error AsmHd6309::encodeBitOperation(const char *line) {
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
    addInsnCode();
    addByte(post);
    return encodeDirect(line);
}

Error AsmHd6309::encodeImmediatePlus(const char *line) {
    if (*line++ != '#') return setError(UNKNOWN_OPERAND);
    uint16_t val;
    if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);

    if (determineAddrMode(line, _addrMode)) return getError();
    switch (_addrMode) {
    case DIRECT_PG: _addrMode = IMM_DIRECT; break;
    case EXTENDED: _addrMode = IMM_EXTENDED; break;
    case INDEXED: _addrMode = IMM_INDEXED; break;
    default: return setError(UNKNOWN_OPERAND);
    }
    if (_tableHd6309.search(*this, _addrMode)) return setError(UNKNOWN_INSTRUCTION);
    addInsnCode();
    addByte((uint8_t)val);
    switch (_addrMode) {
    case IMM_DIRECT: return encodeDirect(line);
    case IMM_EXTENDED: return encodeExtended(line);
    case IMM_INDEXED: return encodeIndexed(line);
    default: return setError(UNKNOWN_OPERAND);
    }
}

Error AsmHd6309::encodeTransferMemory(const char *line) {
    constexpr host::uint_t baseLen = sizeof(tableTfmBaseReg) / sizeof(tableTfmBaseReg[0]);
    RegName regName;
    if ((regName = parseRegName(line, tableTfmBaseReg, baseLen)) == NONE)
        return setError(UNKNOWN_REGISTER);
    line += regNameLen(regName);
    char srcMode = 0;
    if (*line == '+' || *line == '-') srcMode = *line++;
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    target::opcode_t post = encodeRegNumber(regName, tableTfmBaseReg, baseLen) << 4;

    if ((regName = parseRegName(line, tableTfmBaseReg, baseLen)) == NONE)
        return setError(UNKNOWN_REGISTER);
    line += regNameLen(regName);
    char dstMode = 0;
    if (*line == '+' || *line == '-') dstMode = *line++;
    if (*skipSpace(line)) return setError(GARBAGE_AT_END);
    post |= encodeRegNumber(regName, tableTfmBaseReg, baseLen);

    for (uint8_t mode = 0; mode < sizeof(tableTfmSrcMode); mode++) {
        if (srcMode == tableTfmSrcMode[mode] && dstMode == tableTfmDstMode[mode]) {
            target::opcode_t prefixCode = _tableHd6309.prefixCode(insnCode());
            _insnCode = _tableHd6309.insnCode(prefixCode, 0x38 + mode);
            addInsnCode();
            addByte(post);
            return setError(OK);
        }
    }
    return setError(UNKNOWN_OPERAND);
}

Error AsmHd6309::encode(target::uintptr_t addr, const char *line, SymbolTable *symtab) {
    reset(addr, symtab);
    line = skipSpace(line);
    if (!*line) return setError(NO_TEXT);
    line = copyWord(line, _name, sizeof(_name) - 1);

    if (_tableHd6309.search(*this, _name)) return setError(UNKNOWN_INSTRUCTION);

    switch (_addrMode) {
    case INHERENT:
        addInsnCode();
        return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
    case RELATIVE:
        return encodeRelative(line);
    case STACK_OP:
        return encodeStackOp(line);
    case REGISTERS:
        return encodeRegisters(line);
//#ifdef SUPPORT_HD6309
    case IMM_DIRECT: case IMM_EXTENDED: case IMM_INDEXED:
        return encodeImmediatePlus(line);
    case BIT_OPERATION:
        return encodeBitOperation(line);
    case TRANSFER_MEM:
        return encodeTransferMemory(line);
//#endif
    default:
        break;
    }

    if (determineAddrMode(line, _addrMode)) return getError();
    if (_tableHd6309.search(*this, _addrMode)) return setError(UNKNOWN_INSTRUCTION);
    switch (_addrMode) {
    case IMMEDIATE: return encodeImmediate(line);
    case DIRECT_PG: return encodeDirect(line);
    case EXTENDED: return encodeExtended(line);
    case INDEXED: return encodeIndexed(line);
    default: return setError(UNKNOWN_OPERAND);
    }
}
