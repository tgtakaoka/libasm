/* -*- mode: c++; -*- */
#ifndef __ASM_R65C02_IMPL_H__
#define __ASM_R65C02_IMPL_H__

#include <ctype.h>

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
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

template<McuType mcuType>
Error Assembler<mcuType>::getOperand16(const char *&in, target::uint16_t &val) const {
    if (*in == '$') {
        in++;
        return getHex16(in, val);
    }
    if (getInt16(in, val) == OK) return OK;
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

template<McuType mcuType>
Error Assembler<mcuType>::encodeRelative(
    const char *line, Insn &insn, bool emitInsn) {
    target::uintptr_t addr;
    if (getOperand16(line, addr)) return setError(UNKNOWN_OPERAND);
    const target::uintptr_t base = insn.address() + (emitInsn ? 2 : 3);
    const target::ptrdiff_t delta = addr - base;
    if (emitInsn) emitInsnCode(insn);
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(target::byte_t(delta));
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeZeroPageRelative(const char *line, Insn &insn) {
    if (*line == '<') line++;
    target::uint16_t zp;
    if (getOperand16(line, zp)) return setError(UNKNOWN_OPERAND);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    emitInsnCode(insn);
    insn.emitByte(zp);
    return encodeRelative(line, insn, /* emitInsn */ false);
}

template<McuType mcuType>
Error Assembler<mcuType>::parseOperand(
    const char *line, Insn &insn, target::uint16_t &val) {
    char c = toupper(*line);
    if (c == '#') {
        line++;
        if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
        if (*skipSpace(line)) return setError(GARBAGE_AT_END);
        insn.setAddrMode(IMMEDIATE);
        return OK;
    }
    if (c == 'A' && *skipSpace(line + 1) == 0) {
        insn.setAddrMode(ACCUMULATOR);
        return OK;
    }
    const bool indirect = (c == '(');
    if (indirect) line++;
    const char mode = *line;
    if (mode == '<' || mode == '>') line++;
    if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
    if (!indirect && *skipSpace(line) == 0) {
        if (mode == '>' || val >= 0x0100) {
            insn.setAddrMode(ABSOLUTE);
            return OK;
        }
        if (mode == '<' || val < 0x0100) {
            insn.setAddrMode(ZEROPAGE);
            return OK;
        }
        return setError(OPERAND_TOO_FAR);
    }

    c = *line++;
    if (c == ')' && indirect) {
        if (*skipSpace(line) == 0) {
            if (mode == '>' || val >= 0x0100) {
                insn.setAddrMode(ABS_INDIRECT);
                return OK;
            }
            if (mode == '<' || val < 0x0100) {
                insn.setAddrMode(ZP_INDIRECT);
                return OK;
            }
            return setError(OPERAND_NOT_ZP);
        }
        if (*line++ != ',')
            return setError(UNKNOWN_OPERAND);
        if (toupper(*line) == 'Y' && *skipSpace(line + 1) == 0) {
            if (mode == '<' || val < 0x0100) {
                insn.setAddrMode(INDIRECT_IDX);
                return OK;
            }
            return setError(OPERAND_NOT_ZP);
        }
        return setError(UNKNOWN_OPERAND);
    }

    if (c != ',') return setError(UNKNOWN_OPERAND);
    const char index = toupper(*line++);
    if (index != 'X' && index != 'Y') return setError(UNKNOWN_OPERAND);

    if (!indirect && *skipSpace(line) == 0) {
        if (mode == '>' || val >= 0x0100)  {
            insn.setAddrMode(index == 'X' ? ABS_IDX_X : ABS_IDX_Y);
            return OK;
        }
        if (mode == '<' || val < 0x100) {
            insn.setAddrMode(index == 'X' ? ZP_IDX_X  : ZP_IDX_Y);
            return OK;
        }
        return setError(OPERAND_NOT_ZP);
    }
    if (indirect && index == 'X' && *line == ')' && *skipSpace(line + 1) == 0) {
        insn.setAddrMode(INDX_IND);
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

template<McuType mcuType>
Error Assembler<mcuType>::encode(
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

    if (InsnTable<mcuType>::table()->searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    if (insn.mcuType() == R65C02 && mcuType != R65C02)
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case IMPLIED:
        emitInsnCode(insn);
        return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
    case REL8:
        return encodeRelative(line, insn, /* emitInsn */ true);
    default:
        if (mcuType == R65C02 && insn.addrMode() == ZP_REL8)
            return encodeZeroPageRelative(line, insn);
        break;
    }

    target::uint16_t val;
    if (parseOperand(line, insn, val)) return getError();
    if (InsnTable<mcuType>::table()->searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case ACCUMULATOR:
        emitInsnCode(insn);
        return setError(OK);
    case IMMEDIATE:
    case ZEROPAGE:
    case ZP_IDX_X:
    case ZP_IDX_Y:
    case INDX_IND:
    case INDIRECT_IDX:
    case ZP_INDIRECT:
        emitInsnCode(insn);
        insn.emitByte(val);
        return setError(OK);
    case ABSOLUTE:
    case ABS_IDX_X:
    case ABS_IDX_Y:
    case ABS_INDIRECT:
    case IDX_ABS_IND:
        emitInsnCode(insn);
        insn.emitUint16(val);
        return setError(OK);
    default:
        return setError(INTERNAL_ERROR);
    }
}

#endif // __ASM_R65C02_IMPL_H__
