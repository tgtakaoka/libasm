#include <ctype.h>

#include "asm_z80.h"

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

Error AsmZ80::getInt16(uint16_t &val) {
    uint16_t v = 0;
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
    if (sign == '-') v = -(int16_t)v;
    val = v;
    _scan = p;
    return OK;
}

Error AsmZ80::getOperand16(uint16_t &val) {
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
    OprFormat &oprFormat, RegName &regName, uint16_t &operand) {
    setError(OK);

    if (oprFormat == COND_4 || oprFormat == COND_8) {
        CcName ccName;
        if ((ccName = RegZ80::parseCc4Name(_scan)) != CC_UNDEF) {
            _scan += RegZ80::ccNameLen(ccName);
            oprFormat = COND_4;
            operand = RegZ80::encodeCcName(ccName);
            return OK;
        }
        if ((ccName = RegZ80::parseCc8Name(_scan)) != CC_UNDEF) {
            _scan += RegZ80::ccNameLen(ccName);
            oprFormat = COND_8;
            operand = RegZ80::encodeCcName(ccName);
            return OK;
        }
    }

    regName = REG_UNDEF;
    operand = 0;
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
        return OK;
    }
    if (*_scan == '(') {
        regName = RegZ80::parseRegister(++_scan);
        if (regName == REG_UNDEF) {
            if (getOperand16(operand) || *_scan != ')')
                return setError(UNKNOWN_OPERAND);
            _scan++;
            oprFormat = (operand < 0x100) ? ADDR_8 : ADDR_16;
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
            return OK;
        }
        if (*_scan == '+' || *_scan == '-') {
            if ((regName == REG_IX || regName == REG_IY)
                && getOperand16(operand) == OK && *_scan == ')') {
                _scan++;
                const int16_t offset = int16_t(operand);
                if (offset >= -128 && offset < 128) {
                    oprFormat = IX_OFF;
                    return OK;
                }
            }
        }
        return setError(UNKNOWN_OPERAND);
    }
    if (getOperand16(operand) == OK) {
        if (operand < 0x100) oprFormat = IMM_8;
        else oprFormat = IMM_16;
        return setError(OK);
    }
    return setError(UNKNOWN_OPERAND);
}

Error AsmZ80::encode(
    const char *line, Insn &insn, target::uintptr_t addr,
    SymbolTable<target::uintptr_t> *symtab) {
    reset(skipSpace(line), symtab);
    insn.resetAddress(addr);
    if (!*_scan) return setError(NO_TEXT);
    const char *endName;
    for (endName = _scan; isidchar(*endName); endName++)
        ;
    insn.setName(_scan, endName);
    _scan = skipSpace(endName);

    if (TableZ80.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);

    OprFormat leftFormat = insn.leftFormat();
    OprFormat rightFormat = insn.rightFormat();
    RegName leftReg, rightReg;
    uint16_t leftOpr, rightOpr;
    if (parseOperand(leftFormat, leftReg, leftOpr))
        return getError();
    if (*_scan == ',') {
        _scan = skipSpace(_scan + 1);
        if (parseOperand(rightFormat, rightReg, rightOpr))
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
