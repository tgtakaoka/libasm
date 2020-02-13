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
    if (emitInsn) insn.emitInsn();
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
    insn.emitInsn();
    insn.emitByte(zp);
    return encodeRelative(insn, /* emitInsn */ false);
}
#endif

Error AsmM6502::selectMode(char modifier, Operand &op, AddrMode abs, AddrMode zp) {
    if (modifier == '>' || op.val16 >= 0x100) {
        op.mode = abs;
        return OK;
    }
    if (modifier == '<' || op.val16 < 0x100) {
        op.mode = zp;
        return OK;
    }
    return op.setError(OPERAND_NOT_ZP);
}

Error AsmM6502::parseOperand(Operand &op) {
    const char *p = _scan;
    if (*p == '#') {            // #nn
        uint8_t val8;
        _scan = p + 1;
        if (getOperand8(val8)) return op.setError(getError());
        p = _scan;
        op.mode = IMMEDIATE;
        op.val16 = val8;
        op.setError(OK);
    } else if (_regs.compareRegName(p, REG_A)) { // A
        p += _regs.regNameLen(REG_A);
        op.mode = ACCUMULATOR;
        op.setError(OK);
    } else {
        const bool indirect = (*p == '(');
        if (indirect) p = skipSpaces(p + 1);
        const char modifier = *p;
        if (modifier == '<' || modifier == '>') p++;
        _scan = p;
        if (getOperand16(op.val16)) return op.setError(getError());
        op.setError(getError());
        p = skipSpaces(_scan);
        if (*p == ',') {
            p = skipSpaces(p + 1);
            if (_regs.compareRegName(p, REG_X)) { // nn,X, (nn,X)
                p = skipSpaces(p + _regs.regNameLen(REG_X));
                if (indirect) {
                    if (*p != ')') return op.setError(UNKNOWN_OPERAND);
                    p++;
                    if (selectMode(modifier, op, IDX_ABS_IND, INDX_IND))
                        return op.getError();
                    if (op.mode == IDX_ABS_IND && !TableM6502.is65c02())
                        return op.setError(UNKNOWN_OPERAND);
                } else {
                    if (selectMode(modifier, op, ABS_IDX_X, ZP_IDX_X))
                        return op.getError();
                }
            } else if (_regs.compareRegName(p, REG_Y)) { // nn.Y
                if (indirect) return op.setError(UNKNOWN_OPERAND);
                p = skipSpaces(p + _regs.regNameLen(REG_Y));
                if (selectMode(modifier, op, ABS_IDX_Y, ZP_IDX_Y))
                    return op.getError();
            } else return op.setError(UNKNOWN_OPERAND);
        } else if (*p == ')') {
            if (!indirect) return op.setError(UNKNOWN_OPERAND);
            p = skipSpaces(p + 1);
            if (*p == ',') {
                p = skipSpaces(p + 1);
                if (_regs.compareRegName(p, REG_Y)) { // (zp),Y
                    p += _regs.regNameLen(REG_Y);
                    if (modifier == '<' || op.val16 < 0x100) {
                        op.mode = INDIRECT_IDX;
                    } else return op.setError(OPERAND_NOT_ZP);
                } else return op.setError(UNKNOWN_OPERAND);
            } else {            // (abs), (zp)
                if (selectMode(modifier, op, ABS_INDIRECT, ZP_INDIRECT))
                    return op.getError();
                if (op.mode == ZP_INDIRECT && !TableM6502.is65c02())
                    return op.setError(UNKNOWN_OPERAND);
            }
        } else {                // abs, zp
            if (indirect) return op.setError(UNKNOWN_OPERAND);
            if (selectMode(modifier, op, ABSOLUTE, ZEROPAGE))
                return op.getError();
        }
    }
    _scan = skipSpaces(p);
    return OK;
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
        insn.emitInsn();
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

    Operand op;
    if (parseOperand(op)) return setError(op);
    insn.setAddrMode(op.mode);
    if (TableM6502.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case ACCUMULATOR:
        insn.emitInsn();
        break;
    case IMMEDIATE:
    case ZEROPAGE:
    case ZP_IDX_X:
    case ZP_IDX_Y:
    case INDX_IND:
    case INDIRECT_IDX:
    case ZP_INDIRECT:
        insn.emitInsn();
        insn.emitByte(static_cast<uint8_t>(op.val16));
        break;
    case ABSOLUTE:
    case ABS_IDX_X:
    case ABS_IDX_Y:
    case ABS_INDIRECT:
    case IDX_ABS_IND:
        insn.emitInsn();
        insn.emitUint16(op.val16);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }

    return checkLineEnd();
}
