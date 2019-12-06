#include <ctype.h>

#include "asm_i8080.h"

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

Error Assembler::getInt16(target::uint16_t &val) {
    target::uint16_t v = 0;
    const char *p;

    for (p = _scan; isxdigit(*p); p++)
        ;
    if (p > _scan && toupper(*p) == 'H') {
        for (p = _scan; isxdigit(*p); p++) {
            v <<= 4;
            v += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        }
        val = v;
        _scan = ++p;
        return OK;
    }

    for (p = _scan; *p >= '0' && *p < '8'; p++)
        ;
    if (p > _scan && toupper(*p) == 'O') {
        for (p = _scan; *p >= '0' && *p < '8'; p++) {
            v <<= 3;
            v += *p - '0';
        }
        val = v;
        _scan = ++p;
        return OK;
    }

    for (p = _scan; *p == '0' || *p == '1'; p++)
        ;
    if (p > _scan && toupper(*p) == 'B') {
        for (p = _scan; *p == '0' || *p == '1'; p++) {
            v <<= 1;
            v += *p - '0';
        }
        val = v;
        _scan = ++p;
        return OK;
    }

    p = _scan;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    while (isdigit(*p)) {
        v *= 10;
        v += *p++ - '0';
    }
    if (sign == '-') v = -(target::int16_t)v;
    val = v;
    _scan = p;
    return OK;
}

Error Assembler::getOperand16(target::uint16_t &val) {
    if (getInt16(val) == OK) return setError(OK);
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isidchar(_scan[idx]); idx++) {
        symbol_buffer[idx] = _scan[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        _scan += idx;
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

Error Assembler::checkLineEnd() {
    if (*skipSpace(_scan) == 0) return setError(OK);
    return setError(GARBAGE_AT_END);
}

Error Assembler::encodePointerReg(Insn &insn) {
    const RegName regName = Registers::parsePointerReg(_scan);
    const host::int_t num = Registers::encodePointerReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    insn.setInsnCode(insn.insnCode() | (num << 4));
    _scan += Registers::regNameLen(regName);
    return setError(OK);
}

Error Assembler::encodeStackReg(Insn &insn) {
    const RegName regName = Registers::parseStackReg(_scan);
    const host::int_t num = Registers::encodeStackReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    insn.setInsnCode(insn.insnCode() | (num << 4));
    _scan += Registers::regNameLen(regName);
    return setError(OK);
}

Error Assembler::encodeIndexReg(Insn &insn) {
    const RegName regName = Registers::parseIndexReg(_scan);
    const host::int_t num = Registers::encodeIndexReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    insn.setInsnCode(insn.insnCode() | (num << 4));
    _scan += Registers::regNameLen(regName);
    return setError(OK);
}

Error Assembler::encodeDataReg(Insn &insn) {
    const RegName regName = Registers::parseDataReg(_scan);
    const host::int_t num = Registers::encodeDataReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    if (insn.insnFormat() == DATA_REG)
        insn.setInsnCode(insn.insnCode() | (num << 3));
    if (insn.insnFormat() == LOW_DATA_REG)
        insn.setInsnCode(insn.insnCode() | num);
    _scan += Registers::regNameLen(regName);
    return setError(OK);
}

Error Assembler::encodeDataDataReg(Insn &insn) {
    const RegName dstReg = Registers::parseDataReg(_scan);
    if (dstReg == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    _scan += Registers::regNameLen(dstReg);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    const RegName srcReg = Registers::parseDataReg(++_scan);
    if (srcReg == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    _scan += Registers::regNameLen(srcReg);

    const host::uint_t dstNum = Registers::encodeDataReg(dstReg);
    const host::uint_t srcNum = Registers::encodeDataReg(srcReg);
    insn.setInsnCode(insn.insnCode() | (dstNum << 3) | srcNum);
    return setError(OK);
}

Error Assembler::encodeVectorNo(Insn &insn) {
    target::uint16_t vecNo;
    if (getOperand16(vecNo) == OK && vecNo < 8) {
        insn.setInsnCode(insn.insnCode() | (vecNo << 3));
        return setError(OK);
    }
    return setError(UNKNOWN_OPERAND);
}

Error Assembler::encodeImmediate(Insn &insn) {
    if (insn.insnFormat() != NO_FORMAT && *_scan++ != ',')
        return setError(UNKNOWN_OPERAND);
    target::uint16_t val;
    if (getOperand16(val)) return getError();
    if (insn.addrMode() == IMM8)
        insn.emitByte(val);
    if (insn.addrMode() == IMM16)
        insn.emitUint16(val);
    return checkLineEnd();
}

Error Assembler::encodeDirect(Insn &insn) {
    target::uint16_t addr;
    if (getOperand16(addr)) return getError();
    insn.emitUint16(addr);
    return checkLineEnd();
}

Error Assembler::encodeIoaddr(Insn &insn) {
    target::uint16_t addr;
    if (getOperand16(addr)) return getError();
    insn.emitByte(addr);
    return checkLineEnd();
}

Error Assembler::encode(
    const char *line, Insn &insn, target::uintptr_t addr, SymbolTable *symtab) {
    reset(line, symtab);
    insn.resetAddress(addr);
    _scan = skipSpace(_scan);
    if (!*_scan) return setError(NO_TEXT);
    const char *endName;
    for (endName = _scan; isidchar(*endName); endName++)
        ;
    insn.setName(_scan, endName);
    _scan = skipSpace(endName);

    if (InsnTable.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.insnFormat()) {
    case NO_FORMAT:     setError(OK); break;
    case POINTER_REG:   encodePointerReg(insn); break;
    case STACK_REG:     encodeStackReg(insn); break;
    case INDEX_REG:     encodeIndexReg(insn); break;
    case DATA_REG:
    case LOW_DATA_REG:  encodeDataReg(insn); break;
    case DATA_DATA_REG: encodeDataDataReg(insn); break;
    case VECTOR_NO:     encodeVectorNo(insn); break;
    default: break;
    }
    if (getError()) return getError();

    insn.emitByte(insn.insnCode());
    switch (insn.addrMode()) {
    case INHR:   return checkLineEnd();
    case IMM8:
    case IMM16:  return encodeImmediate(insn);
    case DIRECT: return encodeDirect(insn);
    case IOADR:  return encodeIoaddr(insn);
    default:     return setError(UNKNOWN_OPERAND);
    }
}
