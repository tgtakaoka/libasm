#include <ctype.h>
#include <string.h>

#include "asm_z80.h"

bool AsmZ80::acceptCpu(const char *cpu) const {
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
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case DST_FMT:
        if (insn.leftFormat() == COND_8) regNum = leftOpr << 3;
        else return setError(INTERNAL_ERROR);
        break;
    case PTR_FMT:
        if (insn.leftFormat() == REG_16) {
            regNum = RegZ80::encodePointerReg(leftReg) << 4;
        } else if (insn.rightFormat() == REG_16) {
            regNum = RegZ80::encodePointerReg(rightReg) << 4;
        } else return setError(INTERNAL_ERROR);
        break;
    case NO_FMT:
        if (insn.leftFormat() == IX_REG)
            TableZ80::encodePrefixCode(insn, leftReg);
        if (insn.rightFormat() == IX_REG)
            TableZ80::encodePrefixCode(insn, rightReg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.setInsnCode(insn.insnCode() | regNum);
    emitInsnCode(insn);
    if (insn.leftFormat() == ADDR_16 || insn.leftFormat() == IMM_16)
        insn.emitUint16(leftOpr);
    if (insn.rightFormat() == ADDR_16 || insn.rightFormat() == IMM_16)
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
    if (insn.leftFormat() == IX_OFF)
        TableZ80::encodePrefixCode(insn, leftReg);
    if (insn.rightFormat() == IX_OFF)
        TableZ80::encodePrefixCode(insn, rightReg);

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
    if (insn.leftFormat() == IX_OFF)
        insn.emitByte(leftOpr);
    if (insn.rightFormat() == IX_OFF)
        insn.emitByte(rightOpr);
    if (insn.rightFormat() == IMM_8)
        insn.emitByte(rightOpr);
    return setError(OK);
}

Error AsmZ80::encodeIndexedImmediate8(
    Insn &insn, RegName leftReg, RegName rightReg,
    target::uintptr_t leftOpr, target::uintptr_t rightOpr) {
    const target::opcode_t prefixCode = TableZ80::prefixCode(insn.insnCode());
    target::opcode_t opc = TableZ80::opCode(insn.insnCode());
    insn.setInsnCode(prefixCode);
    if (insn.leftFormat() == IX_OFF)
        TableZ80::encodePrefixCode(insn, leftReg);
    if (insn.rightFormat() == IX_OFF)
        TableZ80::encodePrefixCode(insn, rightReg);
    if (insn.insnFormat() == DST_FMT)
        opc |= leftOpr << 3;
    emitInsnCode(insn);
    if (insn.leftFormat() == IX_OFF)
        insn.emitByte(leftOpr);
    if (insn.rightFormat() == IX_OFF)
        insn.emitByte(rightOpr);
    insn.emitByte(opc);
    return setError(OK);
}

Error AsmZ80::encodeInherent(
    Insn &insn, RegName leftReg, RegName rightReg, uint16_t leftOpr) {
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case NO_FMT:
        if (insn.leftFormat() == IX_REG || insn.leftFormat() == IX_PTR)
            TableZ80::encodePrefixCode(insn, leftReg);
        if (insn.rightFormat() == IX_REG)
            TableZ80::encodePrefixCode(insn, rightReg);
        break;
    case DST_SRC_FMT:
        if (insn.leftFormat() == REG_8 || insn.leftFormat() == HL_PTR) {
            regNum = RegZ80::encodeDataReg(leftReg);
        } else if (insn.leftFormat() == BIT_NO) {
            if (leftOpr >= 8) return setError(ILLEGAL_OPERAND);
            regNum = leftOpr;
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        if (insn.rightFormat() == REG_8 || insn.rightFormat() == HL_PTR) {
            regNum |= RegZ80::encodeDataReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        break;
    case DST_FMT:
        if (insn.leftFormat() == REG_8) {
            regNum = RegZ80::encodeDataReg(leftReg);
        } else if (insn.rightFormat() == REG_8) {
            regNum = RegZ80::encodeDataReg(rightReg);
        } else if (insn.leftFormat() == COND_8) {
            regNum = leftOpr;
        } else if (insn.leftFormat() == IMM_NO) {
            if (leftOpr == 0) regNum = 0;
            else if (leftOpr == 1) regNum = 2;
            else if (leftOpr == 2) regNum = 3;
            else return setError(ILLEGAL_OPERAND);
        } else if (insn.leftFormat() == VEC_NO) {
            if ((leftOpr & ~0x38) != 0) return setError(ILLEGAL_OPERAND);
            regNum = leftOpr >> 3;
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        break;
    case SRC_FMT:
        if (insn.leftFormat() == REG_8) {
            regNum = RegZ80::encodeDataReg(leftReg);
        } else if (insn.rightFormat() == REG_8) {
            regNum = RegZ80::encodeDataReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_FMT:
        if (insn.leftFormat() == IR_REG) {
            regNum = RegZ80::encodeIrReg(leftReg);
        } else if (insn.rightFormat() == IR_REG) {
            regNum = RegZ80::encodeIrReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        break;
    case IDX_FMT:
        if (insn.leftFormat() == BC_PTR) {
            regNum = RegZ80::encodeIndexReg(leftReg);
        } else if (insn.rightFormat() == BC_PTR) {
            regNum = RegZ80::encodeIndexReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 4;
        break;
    case PTR_FMT:
        if (insn.leftFormat() == IX_REG)
            TableZ80::encodePrefixCode(insn, leftReg);
        if (insn.leftFormat() == STK_16) {
            regNum = RegZ80::encodeStackReg(leftReg);
        } else if (insn.leftFormat() == REG_16) {
            regNum = RegZ80::encodePointerReg(leftReg);
        } else if (insn.rightFormat() == REG_16) {
            regNum = RegZ80::encodePointerReg(rightReg);
        } else if (insn.rightFormat() == REG_16X) {
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

Error AsmZ80::parseOperand(
    OprFormat &oprFormat, RegName &regName, uint16_t &opr16,
    OprSize &oprSize, AddrMode addrMode) {
    setError(OK);

    if (oprFormat == COND_4 || oprFormat == COND_8) {
        CcName ccName;
        if ((ccName = RegZ80::parseCc4Name(_scan)) != CC_UNDEF) {
            _scan += RegZ80::ccNameLen(ccName);
            oprFormat = COND_4;
            opr16 = RegZ80::encodeCcName(ccName);
            return OK;
        }
        if ((ccName = RegZ80::parseCc8Name(_scan)) != CC_UNDEF) {
            _scan += RegZ80::ccNameLen(ccName);
            oprFormat = COND_8;
            opr16 = RegZ80::encodeCcName(ccName);
            return OK;
        }
    }

    regName = REG_UNDEF;
    opr16 = 0;
    if (*_scan == 0) {
        oprFormat = NO_OPR;
        return OK;
    }

    regName = RegZ80::parseRegister(_scan);
    if (regName != REG_UNDEF) {
        _scan += RegZ80::regNameLen(regName);
        switch (regName) {
        case REG_A:   oprFormat = A_REG; break;
        case REG_BC:  oprFormat = BC_REG; break;
        case REG_DE:  oprFormat = DE_REG; break;
        case REG_HL:  oprFormat = HL_REG; break;
        case REG_SP:  oprFormat = SP_REG; break;
        case REG_IX:
        case REG_IY:  oprFormat = IX_REG; break;
        case REG_I:
        case REG_R:   oprFormat = IR_REG; break;
        case REG_AF:  oprFormat = AF_REG; break;
        case REG_AFP: oprFormat = AFPREG; break;
        default:      oprFormat = REG_8; break;
        }
        if (oprSize == SZ_NONE)
            oprSize = RegZ80::registerSize(regName);
        return OK;
    }
    if (*_scan == '(') {
        regName = RegZ80::parseRegister(++_scan);
        if (regName == REG_UNDEF) {
            if (addrMode == IOADR) {
                uint8_t val8;
                if (getOperand8(val8)) return getError();
                opr16 = val8;
                oprFormat = ADDR_8;
            } else {
                if (getOperand16(opr16)) return getError();
                oprFormat = ADDR_16;
            }
            if (*_scan != ')') return setError(UNKNOWN_OPERAND);
            _scan++;
            return OK;
        }
        _scan += RegZ80::regNameLen(regName);
        if (*_scan == ')') {
            _scan++;
            switch (regName) {
            case REG_BC:
            case REG_DE: oprFormat = BC_PTR; break;
            case REG_HL: oprFormat = HL_PTR; break;
            case REG_SP: oprFormat = SP_PTR; break;
            case REG_IX:
            case REG_IY: oprFormat = IX_PTR; break;
            case REG_C:  oprFormat = C_PTR; break;
            default:     return setError(UNKNOWN_OPERAND);
            }
            if (oprSize == SZ_NONE) oprSize = SZ_BYTE;
            return OK;
        }
        if (*_scan == '+' || *_scan == '-') {
            if (regName == REG_IX || regName == REG_IY) {
                uint8_t val8;
                if (getOperand8(val8)) return getError();
                if (*_scan != ')') return setError(UNKNOWN_OPERAND);
                _scan++;
                opr16 = val8;
                oprFormat = IX_OFF;
                if (oprSize == SZ_NONE) oprSize = SZ_BYTE;
                return OK;
            }
        }
        return setError(UNKNOWN_OPERAND);
    }
    if (oprSize == SZ_BYTE || oprFormat == IMM_NO
        || oprFormat == VEC_NO || oprFormat == BIT_NO) {
        uint8_t val8;
        if (getOperand8(val8)) return getError();
        opr16 = val8;
        oprFormat = IMM_8;
        return OK;
    } else if (oprSize == SZ_WORD || addrMode == REL8 || addrMode == DIRECT) {
        if (getOperand16(opr16)) return getError();
        oprFormat = IMM_16;
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

Error AsmZ80::encode(
    const char *line, Insn &insn, target::uintptr_t addr,
    SymbolTable *symtab) {
    reset(skipSpaces(line), symtab);
    insn.resetAddress(addr);

    if (checkLineEnd() == OK) return setError(NO_INSTRUCTION);
    const char *endName = _parser.readSymbol(_scan, nullptr, nullptr);
    insn.setName(_scan, endName);

    if (TableZ80.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    OprFormat leftFormat = insn.leftFormat();
    OprFormat rightFormat = insn.rightFormat();
    RegName leftReg, rightReg;
    uint16_t leftOpr, rightOpr;
    OprSize oprSize = SZ_NONE;
    if (parseOperand(leftFormat, leftReg, leftOpr, oprSize, insn.addrMode()))
        return getError();
    if (*_scan == ',') {
        _scan = skipSpaces(_scan + 1);
        if (parseOperand(
                rightFormat, rightReg, rightOpr, oprSize, insn.addrMode()))
            return getError();
    } else {
        rightFormat = NO_OPR;
        rightReg = REG_UNDEF;
        rightOpr = 0;
    }

    if (TableZ80.searchNameAndOprFormats(insn, leftFormat, rightFormat))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHR:
        return encodeInherent(insn, leftReg, rightReg, leftOpr);
    case IMM8:
    case IMM16:
        return encodeImmediate(insn, leftReg, rightOpr);
    case DIRECT:
        return encodeDirect(insn, leftReg, rightReg, leftOpr, rightOpr);
    case IOADR:
        return encodeIoaddr(insn, leftOpr, rightOpr);
    case REL8:
        return encodeRelative(insn, leftOpr, rightOpr);
    case INDX:
    case INDX_IMM8:
        if (insn.addrMode() == INDX || insn.rightFormat() == IMM_8) {
            return encodeIndexed(insn, leftReg, rightReg, leftOpr, rightOpr);
        } else {
            return encodeIndexedImmediate8(insn, leftReg, rightReg, leftOpr, rightOpr);
        }
    default: break;
    }

    return setError(UNKNOWN_INSTRUCTION);
}
