#include <stdio.h>

#include <ctype.h>

#include "asm_tms9995.h"

static bool isIdChar(const char c) {
    return isalnum(c) || c == '_';
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

Error Assembler::checkComma() {
    _scan = skipSpace(_scan);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan = skipSpace(_scan + 1);
    return OK;
}

Error Assembler::getHex16(uint16_t &val) {
    const char *p = _scan;
    if (!isxdigit(*p)) return UNKNOWN_OPERAND;
    uint16_t v = 0;
    while (isxdigit(*p)) {
        v <<= 4;
        v += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        p++;
    }
    val = v;
    _scan = p;
    return OK;
}

Error Assembler::getInt16(uint16_t &val) {
    const char *p = _scan;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    int16_t v = 0;
    while (isdigit(*p)) {
        v *= 10;
        v += *p - '0';
        p++;
    }
    if (sign == '-') v = -v;
    val = (uint16_t)v;
    _scan = p;
    return OK;
}

Error Assembler::getOperand16(uint16_t &val) {
    if (*_scan == '>') {
        _scan++;
        return setError(getHex16(val));
    }
    if (getInt16(val) == OK) return setError(OK);
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isIdChar(_scan[idx]); idx++) {
        symbol_buffer[idx] = _scan[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        _scan += idx;
        return setError(OK);
    }
    return setError(UNKNOWN_OPERAND);
}

Error Assembler::parseRegName(uint8_t &regno) {
    const char *line = _scan;
    if (toupper(*line) == 'R' && isdigit(*++line)) {
        if (!isIdChar(line[1])) {
            regno = *line - '0';
            _scan = skipSpace(line + 1);
            return OK;
        } else if (*line == '1' && isdigit(line[1]) && !isIdChar(line[2])) {
            if (line[1] >= '6') return UNKNOWN_OPERAND;
            regno = 10 + line[1] - '0';
            _scan = skipSpace(line + 2);
            return OK;
        }
    }
    return UNKNOWN_OPERAND;
}

Error Assembler::encodeImm(Insn &insn, bool emitInsn) {
    uint16_t val;
    if (getOperand16(val)) return getError();
    if (emitInsn) insn.emitInsn();
    insn.emitOperand(val);
    return setError(OK);
}

Error Assembler::encodeReg(Insn &insn, bool emitInsn) {
    uint8_t regno;
    if (parseRegName(regno)) return setError(UNKNOWN_OPERAND);
    uint16_t operand = regno;
    switch (insn.addrMode()) {
    case REG:
    case REG_IMM:
    case CNT_REG: break;
    case REG_SRC: operand <<= 6; break;
    default: return setError(INTERNAL_ERROR);
    }
    insn.setInsnCode(insn.insnCode() | operand);
    if (emitInsn) insn.emitInsn();
    return setError(OK);
}

Error Assembler::encodeCnt(Insn &insn, bool acceptR0, bool accept16) {
    uint16_t count;
    if (acceptR0 && toupper(_scan[0]) == 'R' && _scan[1] == '0'
        && !isIdChar(_scan[2])) { // R0
        _scan += 2;
        count = 0;
    } else {
        if (getOperand16(count)) return getError();
        if (count > 16 || (!accept16 && count == 16))
            return setError(UNKNOWN_OPERAND);
        count &= 0xf;
    }
    switch (insn.addrMode()) {
    case CNT_REG: count <<= 4; break;
    case CNT_SRC:
    case XOP_SRC: count <<= 6; break;
    default: return setError(INTERNAL_ERROR);
    }
    insn.setInsnCode(insn.insnCode() | count);
    insn.emitInsn();
    return setError(OK);
}

Error Assembler::encodeOpr(Insn &insn, bool emitInsn, bool destinationa) {
    uint8_t regno;
    uint8_t mode = 0;
    uint16_t val;
    if (parseRegName(regno) == OK) {
        mode = 0;
    } else if (*_scan == '*') {
        _scan++;
        mode = 1;
        if (parseRegName(regno)) return setError(UNKNOWN_OPERAND);
        if (*_scan == '+') {
            _scan++;
            mode = 3;
        }
    } else if (*_scan == '@') {
        _scan++;
        mode = 2;
        if (getOperand16(val)) return getError();
        if (*_scan == '(') {
            _scan++;
            if (parseRegName(regno) || regno == 0 || *_scan != ')')
                return setError(UNKNOWN_OPERAND);
            _scan++;
        } else {
            regno = 0;
        }
    }
    uint16_t operand = (mode << 4) | regno;
    if (destinationa) operand <<= 6;
    insn.setInsnCode(insn.insnCode() | operand);
    if (emitInsn)
        insn.emitInsn();
    if (mode == 2)
        insn.emitOperand(val);
    return setError(OK);
}

Error Assembler::encodeRel(Insn &insn) {
    target::uintptr_t addr;
    if (getOperand16(addr) || addr % 2 != 0)
        return setError(UNKNOWN_OPERAND);
    const target::uintptr_t base = insn.address() + 2;
    const target::ptrdiff_t delta = (addr - base) >> 1;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.setInsnCode(insn.insnCode() | (delta & 0xff));
    insn.emitInsn();
    return setError(OK);
}

Error Assembler::encodeCruOff(Insn &insn) {
    uint16_t val;
    if (getOperand16(val)) return getError();
    int16_t offset = (int16_t)val;
    if (offset >= 128 || offset < -128) return setError(UNKNOWN_OPERAND);
    insn.setInsnCode(insn.insnCode() | (offset & 0xff));
    insn.emitInsn();
    return setError(OK);
}

Error Assembler::encode(
    const char *line, Insn &insn, target::uintptr_t addr, SymbolTable *symtab) {
    reset(skipSpace(line), symtab);
    insn.resetAddress(addr);
    if (!*_scan) return setError(NO_TEXT);
    const char *endName;
    for (endName = _scan; isIdChar(*endName); endName++)
        ;
    insn.setName(_scan, endName);
    _scan = skipSpace(endName);

    if (InsnTable.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INH:
        insn.emitInsn();
        setError(OK);
        break;
    case IMM:
        encodeImm(insn, true);
        break;
    case REG:
        encodeReg(insn, true);
        break;
    case REG_IMM:
        if (encodeReg(insn, true)) return getError();
        if (checkComma()) return getError();
        encodeImm(insn, false);
        break;
    case CNT_REG:
        if (encodeReg(insn, false)) return getError();
        if (checkComma()) return getError();
        encodeCnt(insn, /* R0 */true, /* 16 */false);
        break;
    case SRC:
        encodeOpr(insn, true);
        break;
    case CNT_SRC:
        encodeOpr(insn, false);
        if (checkComma()) return getError();
        encodeCnt(insn, /* R0 */false, /* 16 */true);
        break;
    case XOP_SRC:
        encodeOpr(insn, false);
        if (checkComma()) return getError();
        encodeCnt(insn, /* R0 */false, /* 16 */false);
        break;
    case REG_SRC:
        encodeOpr(insn, false);
        if (checkComma()) return getError();
        encodeReg(insn, true);
        break;
    case DST_SRC:
        encodeOpr(insn, false);
        if (checkComma()) return getError();
        encodeOpr(insn, true, true);
        break;
    case REL:
        encodeRel(insn);
        break;
    case CRU_OFF:
        encodeCruOff(insn);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    if (getError()) return getError();
    return *skipSpace(_scan) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}
