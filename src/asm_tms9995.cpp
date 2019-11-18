#include <stdio.h>

#include <ctype.h>

#include "asm_tms9995.h"

static bool isIdChar(const char c) {
    return isalnum(c) || c == '_';
}

static Error getHex16(const char *&in, target::uint16_t &val) {
    const char *p = in;
    if (!isxdigit(*p)) return UNKNOWN_OPERAND;
    val = 0;
    while (isxdigit(*p)) {
        val <<= 4;
        val += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        p++;
    }
    in = p;
    return OK;
}

static Error getInt16(const char *&in, target::uint16_t &val) {
    const char *p = in;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    val = 0;
    while (isdigit(*p)) {
        val *= 10;
        val += *p - '0';
        p++;
    }
    in = p;
    if (sign == '-') val = -(target::int16_t)val;
    return OK;
}

Error Assembler::getOperand16(const char *&in, target::uint16_t &val) {
    if (*in == '>') {
        in++;
        return setError(getHex16(in, val));
    }
    if (getInt16(in, val) == OK) return setError(OK);
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isIdChar(in[idx]); idx++) {
        symbol_buffer[idx] = in[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        in += idx;
        return setError(OK);
    }
    return setError(UNKNOWN_OPERAND);
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

Error Assembler::checkComma(const char *&line) {
    line = skipSpace(line);
    if (*line != ',') return setError(UNKNOWN_OPERAND);
    line = skipSpace(line + 1);
    return OK;
}

#define NO_REG (-1)
static const char *parseRegName(const char *line, target::int8_t &regno) {
    if (toupper(*line) == 'R' && isdigit(*++line)) {
        if (!isIdChar(line[1])) {
            regno = *line - '0';
            return skipSpace(line + 1);
        } else if (*line == '1' && isdigit(line[1]) && !isIdChar(line[2])) {
            regno = 10 + line[1] - '0';
            return skipSpace(line + 2);
        }
    }
    regno = NO_REG;
    return line;
}

Error Assembler::encodeImm(const char *&line, Insn &insn, bool emitInsn) {
    target::uint16_t val;
    if (getOperand16(line, val)) return getError();
    if (emitInsn) insn.emitInsn();
    insn.emitOperand(val);
    return setError(OK);
}

Error Assembler::encodeReg(const char *&line, Insn &insn, bool emitInsn) {
    target::int8_t regno;
    line = parseRegName(line, regno);
    if (regno != NO_REG) {
        target::uint16_t operand = regno;
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
    return setError(UNKNOWN_OPERAND);
}

Error Assembler::encodeCnt(
    const char *&line, Insn &insn, bool acceptR0, bool accept16) {
    target::uint16_t count;
    if (acceptR0 && toupper(line[0]) == 'R' && line[1] == '0'
        && !isIdChar(line[2])) { // R0
        line += 2;
        count = 0;
    } else {
        if (getOperand16(line, count)) return getError();
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

Error Assembler::encodeOpr(
    const char *&line, Insn &insn, bool emitInsn, bool destinationa) {
    target::int8_t regno;
    target::byte_t mode = 0;
    target::uint16_t val;
    line = parseRegName(line, regno);
    if (regno != NO_REG) {
        mode = 0;
    } else if (*line == '*') {
        line++;
        mode = 1;
        line = parseRegName(line, regno);
        if (regno == NO_REG) return setError(UNKNOWN_OPERAND);
        if (*line == '+') {
            line++;
            mode = 3;
        }
    } else if (*line == '@') {
        line++;
        mode = 2;
        if (getOperand16(line, val)) return getError();
        if (*line == '(') {
            line++;
            line = parseRegName(line, regno);
            if (regno == NO_REG || regno == 0 || *line != ')')
                return setError(UNKNOWN_OPERAND);
            line++;
        } else {
            regno = 0;
        }
    }
    target::uint16_t operand = (mode << 4) | regno;
    if (destinationa) operand <<= 6;
    insn.setInsnCode(insn.insnCode() | operand);
    if (emitInsn)
        insn.emitInsn();
    if (mode == 2)
        insn.emitOperand(val);
    return setError(OK);
}

Error Assembler::encodeRel(const char *&line, Insn &insn) {
    target::uintptr_t addr;
    if (getOperand16(line, addr) || addr % 2 != 0)
        return setError(UNKNOWN_OPERAND);
    const target::uintptr_t base = insn.address() + 2;
    const target::ptrdiff_t delta = (addr - base) >> 1;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.setInsnCode(insn.insnCode() | (delta & 0xff));
    insn.emitInsn();
    return setError(OK);
}

Error Assembler::encodeCruOff(const char *&line, Insn &insn) {
    target::uint16_t val;
    if (getOperand16(line, val)) return getError();
    target::int16_t offset = (target::int16_t)val;
    if (offset >= 128 || offset < -128) return setError(UNKNOWN_OPERAND);
    insn.setInsnCode(insn.insnCode() | (offset & 0xff));
    insn.emitInsn();
    return setError(OK);
}

Error Assembler::encode(
    const char *line, Insn &insn, target::uintptr_t addr, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(addr);
    line = skipSpace(line);
    if (!*line) return setError(NO_TEXT);
    const char *endName;
    for (endName = line; isIdChar(*endName); endName++)
        ;
    insn.setName(line, endName);
    line = skipSpace(endName);

    if (InsnTable.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INH:
        insn.emitInsn();
        setError(OK);
        break;
    case IMM:
        encodeImm(line, insn, true);
        break;
    case REG:
        encodeReg(line, insn, true);
        break;
    case REG_IMM:
        if (encodeReg(line, insn, true)) return getError();
        if (checkComma(line)) return getError();
        encodeImm(line, insn, false);
        break;
    case CNT_REG:
        if (encodeReg(line, insn, false)) return getError();
        if (checkComma(line)) return getError();
        encodeCnt(line, insn, /* R0 */true, /* 16 */false);
        break;
    case SRC:
        encodeOpr(line, insn, true);
        break;
    case CNT_SRC:
        encodeOpr(line, insn, false);
        if (checkComma(line)) return getError();
        encodeCnt(line, insn, /* R0 */false, /* 16 */true);
        break;
    case XOP_SRC:
        encodeOpr(line, insn, false);
        if (checkComma(line)) return getError();
        encodeCnt(line, insn, /* R0 */false, /* 16 */false);
        break;
    case REG_SRC:
        encodeOpr(line, insn, false);
        if (checkComma(line)) return getError();
        encodeReg(line, insn, true);
        break;
    case DST_SRC:
        encodeOpr(line, insn, false);
        if (checkComma(line)) return getError();
        encodeOpr(line, insn, true, true);
        break;
    case REL:
        encodeRel(line, insn);
        break;
    case CRU_OFF:
        encodeCruOff(line, insn);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    if (getError()) return getError();
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}
