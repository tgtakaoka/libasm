#include "asm_m6502.h"
#include "table_m6502.h"

#include <ctype.h>
#include <string.h>

bool AsmM6502::acceptCpu(const char *cpu) {
    if (strcmp(cpu, "6502") == 0) {
        TableM6502.setMcuType(M6502);
        return true;
    }
    if (strcasecmp(cpu, "65C02") == 0) {
        TableM6502.setMcuType(W65C02);
        return true;
    }
    return false;
}

Error AsmM6502::encodeRelative(Insn &insn, bool emitInsn) {
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    const target::uintptr_t base = insn.address() + (emitInsn ? 2 : 3);
    const target::ptrdiff_t delta = addr - base;
    if (emitInsn) emitInsnCode(insn);
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(uint8_t(delta));
    return checkLineEnd();
}

#ifdef W65C02_ENABLE_BITOPS
Error AsmM6502::encodeZeroPageRelative(Insn &insn) {
    if (*_scan == '<') _scan++;
    uint16_t zp;
    if (getOperand(zp)) return getError();
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    emitInsnCode(insn);
    insn.emitByte(zp);
    return encodeRelative(insn, /* emitInsn */ false);
}
#endif

Error AsmM6502::parseOperand(Insn &insn, uint16_t &val16) {
    const char *p = _scan;
    char c = toupper(*p);
    if (c == '#') {
        uint8_t val8;
        _scan = p + 1;
        if (getOperand8(val8)) return getError();
        val16 = val8;
        if (checkLineEnd()) return getError();
        insn.setAddrMode(IMMEDIATE);
        return OK;
    }
    if (_regs.compareRegName(_scan, REG_A)) {
        _scan = p + _regs.regNameLen(REG_A);
        insn.setAddrMode(ACCUMULATOR);
        return OK;
    }
    const bool indirect = (c == '(');
    if (indirect) p++;
    const char mode = *p;
    if (mode == '<' || mode == '>') p++;
    _scan = p;
    if (getOperand16(val16)) return getError();
    if (!indirect && checkLineEnd() == OK) {
        if (mode == '>' || val16 >= 0x0100) {
            insn.setAddrMode(ABSOLUTE);
            return OK;
        }
        if (mode == '<' || val16 < 0x0100) {
            insn.setAddrMode(ZEROPAGE);
            return OK;
        }
        return setError(OPERAND_TOO_FAR);
    }

    p = _scan;
    c = *p++;
    if (c == ')' && indirect) {
        if (checkLineEnd(p) == OK) {
            _scan = p;
            if (mode == '>' || val16 >= 0x0100) {
                insn.setAddrMode(ABS_INDIRECT);
                return OK;
            }
            if (mode == '<' || val16 < 0x0100) {
                insn.setAddrMode(ZP_INDIRECT);
                return OK;
            }
            return setError(OPERAND_NOT_ZP);
        }
        if (*p++ != ',')
            return setError(UNKNOWN_OPERAND);
        p = skipSpaces(p);
        if (toupper(*p) == 'Y' && checkLineEnd(p + 1) == OK) {
            if (mode == '<' || val16 < 0x0100) {
                _scan = p + 1;
                insn.setAddrMode(INDIRECT_IDX);
                return OK;
            }
            return setError(OPERAND_NOT_ZP);
        }
        return setError(UNKNOWN_OPERAND);
    }

    if (c != ',') return setError(UNKNOWN_OPERAND);
    const RegName index = _regs.parseIndexReg(p);
    if (index == REG_UNDEF) return setError(UNKNOWN_OPERAND);
    p += _regs.regNameLen(index);

    if (!indirect && checkLineEnd(p) == OK) {
        if (mode == '>' || val16 >= 0x0100)  {
            _scan = p;
            insn.setAddrMode(index == REG_X ? ABS_IDX_X : ABS_IDX_Y);
            return OK;
        }
        if (mode == '<' || val16 < 0x100) {
            _scan = p;
            insn.setAddrMode(index == REG_X ? ZP_IDX_X  : ZP_IDX_Y);
            return OK;
        }
        return setError(OPERAND_NOT_ZP);
    }
    if (indirect && index == REG_X && *p == ')'
        && checkLineEnd(p + 1) == OK) {
        _scan = p + 1;
        insn.setAddrMode(INDX_IND);
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

Error AsmM6502::encode(Insn &insn) {
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableM6502.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    if (insn.is65c02() && !TableM6502.is65c02())
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    switch (insn.addrMode()) {
    case IMPLIED:
        emitInsnCode(insn);
        return checkLineEnd();
    case REL8:
        return encodeRelative(insn, /* emitInsn */ true);
    default:
#ifdef W65C02_ENABLE_BITOPS
        if (TableM6502.is65c02() && insn.addrMode() == ZP_REL8)
            return encodeZeroPageRelative(insn);
#endif
        break;
    }

    uint16_t val16;
    if (parseOperand(insn, val16)) return getError();
    if (TableM6502.searchNameAndAddrMode(insn))
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
        insn.emitByte(static_cast<uint8_t>(val16));
        return setError(OK);
    case ABSOLUTE:
    case ABS_IDX_X:
    case ABS_IDX_Y:
    case ABS_INDIRECT:
    case IDX_ABS_IND:
        emitInsnCode(insn);
        insn.emitUint16(val16);
        return setError(OK);
    default:
        return setError(INTERNAL_ERROR);
    }
}
