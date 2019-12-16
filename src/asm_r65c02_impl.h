/* -*- mode: c++; -*- */
#ifndef __ASM_R65C02_IMPL_H__
#define __ASM_R65C02_IMPL_H__

#include "asm_operand.h"

#include <ctype.h>

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

template<McuType mcuType>
Error Asm6502<mcuType>::checkLineEnd() {
    if (*skipSpace(_scan) == 0) return setError(OK);
    return setError(GARBAGE_AT_END);
}

template<McuType mcuType>
Error Asm6502<mcuType>::getOperand(uint16_t &val) {
    AsmMotoOperand<uint16_t, target::uintptr_t> parser(_symtab);
    const char *p = parser.eval(_scan, val);
    if (!p) return setError(UNKNOWN_OPERAND);
    _scan = p;
    return OK;
}

template<McuType mcuType>
Error Asm6502<mcuType>::encodeRelative(Insn &insn, bool emitInsn) {
    target::uintptr_t addr;
    if (getOperand(addr)) return setError(UNKNOWN_OPERAND);
    const target::uintptr_t base = insn.address() + (emitInsn ? 2 : 3);
    const target::ptrdiff_t delta = addr - base;
    if (emitInsn) emitInsnCode(insn);
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(uint8_t(delta));
    return checkLineEnd();
}

#ifdef R65C02_ENABLE_BITOPS
template<McuType mcuType>
Error Asm6502<mcuType>::encodeZeroPageRelative(Insn &insn) {
    if (*_scan == '<') _scan++;
    uint16_t zp;
    if (getOperand(zp) || *_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    emitInsnCode(insn);
    insn.emitByte(zp);
    return encodeRelative(insn, /* emitInsn */ false);
}
#endif

template<McuType mcuType>
Error Asm6502<mcuType>::parseOperand(Insn &insn, uint16_t &val) {
    char c = toupper(*_scan);
    if (c == '#') {
        _scan++;
        if (getOperand(val)) return setError(UNKNOWN_OPERAND);
        if (checkLineEnd()) return setError(GARBAGE_AT_END);
        insn.setAddrMode(IMMEDIATE);
        return OK;
    }
    if (c == 'A' && *skipSpace(_scan + 1) == 0) {
        insn.setAddrMode(ACCUMULATOR);
        return OK;
    }
    const bool indirect = (c == '(');
    if (indirect) _scan++;
    const char mode = *_scan;
    if (mode == '<' || mode == '>') _scan++;
    if (getOperand(val)) return setError(UNKNOWN_OPERAND);
    if (!indirect && *skipSpace(_scan) == 0) {
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

    c = *_scan++;
    if (c == ')' && indirect) {
        if (*skipSpace(_scan) == 0) {
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
        if (*_scan++ != ',')
            return setError(UNKNOWN_OPERAND);
        if (toupper(*_scan) == 'Y' && *skipSpace(_scan + 1) == 0) {
            if (mode == '<' || val < 0x0100) {
                insn.setAddrMode(INDIRECT_IDX);
                return OK;
            }
            return setError(OPERAND_NOT_ZP);
        }
        return setError(UNKNOWN_OPERAND);
    }

    if (c != ',') return setError(UNKNOWN_OPERAND);
    const char index = toupper(*_scan++);
    if (index != 'X' && index != 'Y') return setError(UNKNOWN_OPERAND);

    if (!indirect && *skipSpace(_scan) == 0) {
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
    if (indirect && index == 'X' && *_scan == ')' && *skipSpace(_scan + 1) == 0) {
        insn.setAddrMode(INDX_IND);
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

template<McuType mcuType>
Error Asm6502<mcuType>::encode(
    const char *line, Insn &insn, target::uintptr_t addr,
    SymbolTable<target::uintptr_t> *symtab) {
    reset(skipSpace(line), symtab);
    insn.resetAddress(addr);
    if (!*_scan) return setError(NO_TEXT);
    const char *endName;
    for (endName = _scan; isidchar(*endName); endName++)
        ;
    insn.setName(_scan, endName);

    if (TableR65c02<mcuType>::table()->searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    if (insn.mcuType() == R65C02 && mcuType != R65C02)
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpace(endName);

    switch (insn.addrMode()) {
    case IMPLIED:
        emitInsnCode(insn);
        return checkLineEnd();
    case REL8:
        return encodeRelative(insn, /* emitInsn */ true);
    default:
#ifdef R65C02_ENABLE_BITOPS
        if (mcuType == R65C02 && insn.addrMode() == ZP_REL8)
            return encodeZeroPageRelative(insn);
#endif
        break;
    }

    uint16_t val;
    if (parseOperand(insn, val)) return getError();
    if (TableR65c02<mcuType>::table()->searchNameAndAddrMode(insn))
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

template<McuType mcuType>
bool Asm6502<mcuType>::isRegister(const char *text) const {
    const char r = toupper(*text++);
    return (r == 'A' || r == 'X' || r == 'Y') && !isidchar(*text);
}

#endif // __ASM_R65C02_IMPL_H__
