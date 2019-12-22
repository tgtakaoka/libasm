#include <ctype.h>

#include "asm_tms9995.h"

static bool isIdChar(const char c) {
    return isalnum(c) || c == '_';
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

Error AsmTms9995::checkComma() {
    _scan = skipSpace(_scan);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan = skipSpace(_scan + 1);
    return OK;
}

Error AsmTms9995::getOperand16(uint16_t &val16) {
    _scan = _parser.eval(_scan, val16, _symtab);
    return setError(_parser.getError());
}

Error AsmTms9995::getOperand8(uint8_t &val8) {
    const char *p = _parser.eval(_scan, val8, _symtab);
    if (!p) return setError(UNKNOWN_OPERAND);
    _scan = p;
    return OK;
}

bool AsmTms9995::isRegister(const char *text) const {
    if (toupper(*text++) != 'R' || !isdigit(*text))
        return false;
    if (!isIdChar(text[1]))
        return true;
    if (*text++ != '1' || isIdChar(text[1]))
        return false;
    return *text >= '0' && *text < '6';
}

Error AsmTms9995::parseRegName(uint8_t &regno) {
    if (!isRegister(_scan)) return UNKNOWN_OPERAND;
    uint8_t v = *++_scan - '0';
    if (isdigit(*++_scan)) {
        v *= 10;
        v += *_scan++ - '0';
    }
    regno = v;
    return OK;
}

Error AsmTms9995::encodeImm(Insn &insn, bool emitInsn) {
    uint16_t val;
    if (getOperand16(val)) return getError();
    if (emitInsn) insn.emitInsn();
    insn.emitOperand(val);
    return setError(OK);
}

Error AsmTms9995::encodeReg(Insn &insn, bool emitInsn) {
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

Error AsmTms9995::encodeCnt(Insn &insn, bool acceptR0, bool accept16) {
    uint16_t count;
    if (acceptR0 && toupper(_scan[0]) == 'R' && _scan[1] == '0'
        && !isIdChar(_scan[2])) { // R0
        _scan += 2;
        count = 0;
    } else {
        uint8_t val8;
        if (getOperand8(val8)) return getError();
        if (val8 > 16 || (!accept16 && val8 == 16))
            return setError(UNKNOWN_OPERAND);
        count = val8 & 0xf;
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

Error AsmTms9995::encodeOpr(Insn &insn, bool emitInsn, bool destinationa) {
    uint8_t regno;
    uint8_t mode = 0;
    uint16_t val16;
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
        if (getOperand16(val16)) return getError();
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
        insn.emitOperand(val16);
    return setError(OK);
}

Error AsmTms9995::encodeRel(Insn &insn) {
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    if (addr % 2) return setError(ILLEGAL_OPERAND);
    const target::uintptr_t base = insn.address() + 2;
    const target::ptrdiff_t delta = (addr - base) >> 1;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.setInsnCode(insn.insnCode() | (delta & 0xff));
    insn.emitInsn();
    return setError(OK);
}

Error AsmTms9995::encodeCruOff(Insn &insn) {
    uint8_t val8;
    if (getOperand8(val8)) return getError();
    insn.setInsnCode(insn.insnCode() | val8);
    insn.emitInsn();
    return setError(OK);
}

Error AsmTms9995::encode(
    const char *line, Insn &insn, target::uintptr_t addr,
    SymbolTable *symtab) {
    reset(skipSpace(line), symtab);
    insn.resetAddress(addr);
    if (!*_scan) return setError(NO_TEXT);
    const char *endName;
    for (endName = _scan; isIdChar(*endName); endName++)
        ;
    insn.setName(_scan, endName);

    if (TableTms9995.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpace(endName);

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
