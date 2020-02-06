#include "asm_z80.h"
#include "table_z80.h"

#include <ctype.h>
#include <string.h>

bool AsmZ80::acceptCpu(const char *cpu) {
    return strcasecmp(cpu, "z80") == 0;
}

Error AsmZ80::encodeImmediate(Insn &insn, RegName leftReg, uint16_t rightOp) {
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case DST_FMT:
        regNum = RegZ80::encodeDataReg(leftReg) << 3;
        break;
    case PTR_FMT:
        regNum = RegZ80::encodePointerReg(leftReg) << 4;
        break;
    case NO_FMT:
        if (insn.leftFormat() == IX_REG)
            TableZ80::encodePrefixCode(insn, leftReg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.setInsnCode(insn.insnCode() | regNum);
    emitInsnCode(insn);
    if (insn.addrMode() == IMM8)
        insn.emitByte(rightOp);
    if (insn.addrMode() == IMM16)
        insn.emitUint16(rightOp);
    return setError(OK);
}

Error AsmZ80::encodeDirect(
    Insn &insn, RegName leftReg, RegName rightReg,
    target::uintptr_t leftOpr, target::uintptr_t rightOpr) {
    const OprFormat left = insn.leftFormat();
    const OprFormat right = insn.rightFormat();
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case DST_FMT:
        if (left == COND_8) regNum = leftOpr << 3;
        else return setError(INTERNAL_ERROR);
        break;
    case PTR_FMT:
        if (left == REG_16) {
            regNum = RegZ80::encodePointerReg(leftReg) << 4;
        } else if (right == REG_16) {
            regNum = RegZ80::encodePointerReg(rightReg) << 4;
        } else return setError(INTERNAL_ERROR);
        break;
    case NO_FMT:
        if (left == IX_REG)
            TableZ80::encodePrefixCode(insn, leftReg);
        if (right == IX_REG)
            TableZ80::encodePrefixCode(insn, rightReg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.setInsnCode(insn.insnCode() | regNum);
    emitInsnCode(insn);
    if (left == ADDR_16 || left == IMM_16)
        insn.emitUint16(leftOpr);
    if (right == ADDR_16 || right == IMM_16)
        insn.emitUint16(rightOpr);
    return setError(OK);
}

Error AsmZ80::encodeIoaddr(
    Insn &insn, uint16_t leftOpr, uint16_t rightOpr) {
    emitInsnCode(insn);
    if (insn.leftFormat() == ADDR_8)
        insn.emitByte(leftOpr);
    if (insn.rightFormat() == ADDR_8)
        insn.emitByte(rightOpr);
    return setError(OK);
}

Error AsmZ80::encodeRelative(
    Insn &insn, target::uintptr_t leftOpr, target::uintptr_t rightOpr) {
    if (insn.insnFormat() == CC4_FMT) {
        insn.setInsnCode(insn.insnCode() | (leftOpr << 3));
        leftOpr = rightOpr;
    }
    const target::ptrdiff_t delta = leftOpr - insn.address() - 2;
    if (delta < -128 || delta >= 128) return setError(ILLEGAL_OPERAND);
    emitInsnCode(insn);
    insn.emitByte(uint8_t(delta));
    return setError(OK);
}

Error AsmZ80::encodeIndexed(
    Insn &insn, RegName leftReg, RegName rightReg,
    target::uintptr_t leftOpr, target::uintptr_t rightOpr) {
    const OprFormat left = insn.leftFormat();
    const OprFormat right = insn.rightFormat();
    if (left == IX_OFF) {
        if (right == REG_8 && rightReg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (IX+n),(HL)
        TableZ80::encodePrefixCode(insn, leftReg);
    }
    if (right == IX_OFF) {
        if (left == REG_8 && leftReg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (HL),(IX+n)
        TableZ80::encodePrefixCode(insn, rightReg);
    }

    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case NO_FMT:
        break;
    case DST_FMT:
        regNum = RegZ80::encodeDataReg(leftReg) << 3;
        break;
    case SRC_FMT:
        regNum = RegZ80::encodeDataReg(rightReg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.setInsnCode(insn.insnCode() | regNum);
    emitInsnCode(insn);
    if (left == IX_OFF)
        insn.emitByte(leftOpr);
    if (right == IX_OFF)
        insn.emitByte(rightOpr);
    if (right == IMM_8)
        insn.emitByte(rightOpr);
    return setError(OK);
}

Error AsmZ80::encodeIndexedImmediate8(
    Insn &insn, RegName leftReg, RegName rightReg,
    target::uintptr_t leftOpr, target::uintptr_t rightOpr) {
    const OprFormat left = insn.leftFormat();
    const OprFormat right = insn.rightFormat();
    const target::opcode_t prefixCode = TableZ80::prefixCode(insn.insnCode());
    target::opcode_t opc = TableZ80::opCode(insn.insnCode());
    insn.setInsnCode(prefixCode);
    if (left == IX_OFF)
        TableZ80::encodePrefixCode(insn, leftReg);
    if (right == IX_OFF)
        TableZ80::encodePrefixCode(insn, rightReg);
    if (insn.insnFormat() == DST_FMT)
        opc |= leftOpr << 3;
    emitInsnCode(insn);
    if (left == IX_OFF)
        insn.emitByte(leftOpr);
    if (right == IX_OFF)
        insn.emitByte(rightOpr);
    insn.emitByte(opc);
    return setError(OK);
}

Error AsmZ80::encodeInherent(
    Insn &insn, RegName leftReg, RegName rightReg, uint16_t leftOpr) {
    const OprFormat left = insn.leftFormat();
    const OprFormat right = insn.rightFormat();
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case NO_FMT:
        if (left == IX_REG || left == IX_PTR)
            TableZ80::encodePrefixCode(insn, leftReg);
        if (right == IX_REG)
            TableZ80::encodePrefixCode(insn, rightReg);
        break;
    case DST_SRC_FMT:
        if (left == REG_8 && leftReg == REG_HL
            && right == REG_8 && rightReg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (HL),(HL)
        if (left == REG_8 || left == HL_PTR) {
            regNum = RegZ80::encodeDataReg(leftReg);
        } else if (left == BIT_NO) {
            if (leftOpr >= 8) return setError(ILLEGAL_OPERAND);
            regNum = leftOpr;
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        if (right == REG_8 || right == HL_PTR) {
            regNum |= RegZ80::encodeDataReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        break;
    case DST_FMT:
        if (left == C_PTR && right == REG_8 && rightReg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (C),(HL)
        if (right == C_PTR && left == REG_8 && leftReg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (HL),(C)
        if (left == REG_8) {
            regNum = RegZ80::encodeDataReg(leftReg);
        } else if (right == REG_8) {
            regNum = RegZ80::encodeDataReg(rightReg);
        } else if (left == COND_8) {
            regNum = leftOpr;
        } else if (left == IMM_NO) {
            if (leftOpr == 0) regNum = 0;
            else if (leftOpr == 1) regNum = 2;
            else if (leftOpr == 2) regNum = 3;
            else return setError(ILLEGAL_OPERAND);
        } else if (left == VEC_NO) {
            if ((leftOpr & ~0x38) != 0) return setError(ILLEGAL_OPERAND);
            regNum = leftOpr >> 3;
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        break;
    case SRC_FMT:
        if (left == REG_8) {
            regNum = RegZ80::encodeDataReg(leftReg);
        } else if (right == REG_8) {
            regNum = RegZ80::encodeDataReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_FMT:
        if (left == IR_REG) {
            regNum = RegZ80::encodeIrReg(leftReg);
        } else if (right == IR_REG) {
            regNum = RegZ80::encodeIrReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        break;
    case IDX_FMT:
        if (left == BC_PTR) {
            regNum = RegZ80::encodeIndexReg(leftReg);
        } else if (right == BC_PTR) {
            regNum = RegZ80::encodeIndexReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 4;
        break;
    case PTR_FMT:
        if (left == IX_REG)
            TableZ80::encodePrefixCode(insn, leftReg);
        if (left == STK_16) {
            regNum = RegZ80::encodeStackReg(leftReg);
        } else if (left == REG_16) {
            regNum = RegZ80::encodePointerReg(leftReg);
        } else if (right == REG_16) {
            regNum = RegZ80::encodePointerReg(rightReg);
        } else if (right == REG_16X) {
            regNum = RegZ80::encodePointerRegIx(rightReg, leftReg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 4;
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.setInsnCode(insn.insnCode() | regNum);
    emitInsnCode(insn);
    return setError(OK);
}

Error AsmZ80::parseOperand(Operand &opr) {
    opr.resetError();

    if (opr.format == COND_4 || opr.format == COND_8) {
        CcName ccName;
        if ((ccName = _regs.parseCc4Name(_scan)) != CC_UNDEF) {
            _scan += RegZ80::ccNameLen(ccName);
            opr.format = COND_4;
            opr.val = RegZ80::encodeCcName(ccName);
            return opr.setError(OK);
        }
        if ((ccName = _regs.parseCc8Name(_scan)) != CC_UNDEF) {
            _scan += RegZ80::ccNameLen(ccName);
            opr.format = COND_8;
            opr.val = RegZ80::encodeCcName(ccName);
            return opr.setError(OK);
        }
    }

    opr.reg = REG_UNDEF;
    opr.val = 0;
    if (*_scan == 0) {
        opr.format = NO_OPR;
        return opr.setError(OK);
    }

    opr.reg = _regs.parseRegister(_scan);
    if (opr.reg != REG_UNDEF) {
        _scan += RegZ80::regNameLen(opr.reg);
        switch (opr.reg) {
        case REG_A:   opr.format = A_REG; break;
        case REG_BC:  opr.format = BC_REG; break;
        case REG_DE:  opr.format = DE_REG; break;
        case REG_HL:  opr.format = HL_REG; break;
        case REG_SP:  opr.format = SP_REG; break;
        case REG_IX:
        case REG_IY:  opr.format = IX_REG; break;
        case REG_I:
        case REG_R:   opr.format = IR_REG; break;
        case REG_AF:  opr.format = AF_REG; break;
        case REG_AFP: opr.format = AFPREG; break;
        default:      opr.format = REG_8; break;
        }
        if (opr.size == SZ_NONE)
            opr.size = RegZ80::registerSize(opr.reg);
        return opr.setError(OK);
    }
    const char *p = _scan;
    if (*p == '(') {
        p = skipSpaces(p + 1);
        opr.reg = _regs.parseRegister(p);
        if (opr.reg == REG_UNDEF) {
            _scan = p;
            if (opr.mode == IOADR) {
                uint8_t val8;
                if (getOperand8(val8)) return opr.setError(getError());
                opr.val = val8;
                opr.format = ADDR_8;
            } else {
                if (getOperand16(opr.val)) return opr.setError(getError());
                opr.format = ADDR_16;
            }
            if (*_scan != ')') return opr.setError(UNKNOWN_OPERAND);
            _scan++;
            return opr.setError(OK);
        }
        p = skipSpaces(p + RegZ80::regNameLen(opr.reg));
        if (*p == ')') {
            p++;
            switch (opr.reg) {
            case REG_BC:
            case REG_DE: opr.format = BC_PTR; break;
            case REG_HL: opr.format = HL_PTR; break;
            case REG_SP: opr.format = SP_PTR; break;
            case REG_IX:
            case REG_IY: opr.format = IX_PTR; break;
            case REG_C:  opr.format = C_PTR; break;
            default:     return opr.setError(UNKNOWN_OPERAND);
            }
            if (opr.size == SZ_NONE) opr.size = SZ_BYTE;
            _scan = p;
            return opr.setError(OK);
        }
        if (*p == '+' || *p == '-') {
            if (opr.reg == REG_IX || opr.reg == REG_IY) {
                uint8_t val8;
                _scan = p;
                if (getOperand8(val8)) return opr.setError(getError());
                if (*_scan != ')') return opr.setError(UNKNOWN_OPERAND);
                _scan++;
                opr.val = val8;
                opr.format = IX_OFF;
                if (opr.size == SZ_NONE) opr.size = SZ_BYTE;
                return opr.setError(OK);
            }
        }
        return opr.setError(UNKNOWN_OPERAND);
    }
    if (opr.size == SZ_WORD || opr.mode == REL8 || opr.mode == DIRECT) {
        if (getOperand16(opr.val)) return opr.setError(getError());
        opr.format = IMM_16;
        return opr.setError(OK);
    }
    uint8_t val8;
    if (getOperand8(val8) == OK) {
        opr.val = val8;
        opr.format = IMM_8;
        return opr.setError(OK);
    }
    return opr.setError(UNKNOWN_OPERAND);
}

Error AsmZ80::encode(Insn &insn) {
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableZ80.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    Operand left, right;
    left.format = insn.leftFormat();
    left.size = SZ_NONE;
    left.mode = insn.addrMode();
    parseOperand(left);
    if (left.getError()) return setError(left);
    if (*_scan == ',') {
        _scan = skipSpaces(_scan + 1);
        right.format = insn.rightFormat();
        right.size = left.size;
        right.mode = insn.addrMode();
        parseOperand(right);
        if (right.getError()) return setError(right);
    } else {
        right.format = NO_OPR;
        right.reg = REG_UNDEF;
        right.val = 0;
    }

    if (TableZ80.searchNameAndOprFormats(insn, left.format, right.format))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHR:
        return encodeInherent(insn, left.reg, right.reg, left.val);
    case IMM8:
    case IMM16:
        if (left.format == IMM_8 && right.format == NO_OPR) {
            // SUB/AND/XOR/OR/CP immediate instruction
            right.val = left.val;
        }
        return encodeImmediate(insn, left.reg, right.val);
    case DIRECT:
        return encodeDirect(insn, left.reg, right.reg, left.val, right.val);
    case IOADR:
        return encodeIoaddr(insn, left.val, right.val);
    case REL8:
        return encodeRelative(insn, left.val, right.val);
    case INDX:
    case INDX_IMM8:
        if (insn.addrMode() == INDX || insn.rightFormat() == IMM_8) {
            return encodeIndexed(insn, left.reg, right.reg, left.val, right.val);
        } else {
            return encodeIndexedImmediate8(insn, left.reg, right.reg, left.val, right.val);
        }
    default: break;
    }

    return setError(UNKNOWN_INSTRUCTION);
}
