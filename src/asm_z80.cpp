#include <ctype.h>

#include "asm_z80.h"

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static Error getInt16(const char *&in, target::uint16_t &val) {
    val = 0;
    const char *p;

    for (p = in; isxdigit(*p); p++)
        ;
    if (p > in && toupper(*p) == 'H') {
        for (p = in; isxdigit(*p); p++) {
            val <<= 4;
            val += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        }
        in = ++p;
        return OK;
    }

    for (p = in; *p >= '0' && *p < '8'; p++)
        ;
    if (p > in && toupper(*p) == 'O') {
        for (p = in; *p >= '0' && *p < '8'; p++) {
            val <<= 3;
            val += *p - '0';
        }
        in = ++p;
        return OK;
    }

    for (p = in; *p == '0' || *p == '1'; p++)
        ;
    if (p > in && toupper(*p) == 'B') {
        for (p = in; *p == '0' || *p == '1'; p++) {
            val <<= 1;
            val += *p - '0';
        }
        in = ++p;
        return OK;
    }

    p = in;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    while (isdigit(*p)) {
        val *= 10;
        val += *p++ - '0';
    }
    in = p;
    if (sign == '-') val = -(target::int16_t)val;
    return OK;
}

Error AsmZ80::getOperand16(const char *&in, target::uint16_t &val) {
    if (getInt16(in, val) == OK) return setError(OK);
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
    return setError(UNKNOWN_OPERAND);
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

Error AsmZ80::encodeImmediate(Insn &insn, RegName leftReg, target::uint16_t rightOp) {
    target::byte_t regNum = 0;
    switch (insn.insnFormat()) {
    case DST_FMT:
        regNum = Registers::encodeDataReg(leftReg) << 3;
        break;
    case PTR_FMT:
        regNum = Registers::encodePointerReg(leftReg) << 4;
        break;
    case NO_FMT:
        if (insn.leftFormat() == IX_REG)
            InsnTable::encodePrefixCode(insn, leftReg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.setInsnCode(insn.insnCode() | regNum);
    emitInsnCode(insn);
    if (insn.addrMode() == IMMEDIATE8)
        insn.emitByte(rightOp);
    if (insn.addrMode() == IMMEDIATE16)
        insn.emitUint16(rightOp);
    return setError(OK);
}

Error AsmZ80::encodeDirect(
    Insn &insn, RegName leftReg, RegName rightReg,
    target::uintptr_t leftOpr, target::uintptr_t rightOpr) {
    target::byte_t regNum = 0;
    switch (insn.insnFormat()) {
    case DST_FMT:
        if (insn.leftFormat() == CC8) regNum = leftOpr << 3;
        else return setError(INTERNAL_ERROR);
        break;
    case PTR_FMT:
        if (insn.leftFormat() == REG16) {
            regNum = Registers::encodePointerReg(leftReg) << 4;
        } else if (insn.rightFormat() == REG16) {
            regNum = Registers::encodePointerReg(rightReg) << 4;
        } else return setError(INTERNAL_ERROR);
        break;
    case NO_FMT:
        if (insn.leftFormat() == IX_REG)
            InsnTable::encodePrefixCode(insn, leftReg);
        if (insn.rightFormat() == IX_REG)
            InsnTable::encodePrefixCode(insn, rightReg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.setInsnCode(insn.insnCode() | regNum);
    emitInsnCode(insn);
    if (insn.leftFormat() == ADDR16 || insn.leftFormat() == IMM16)
        insn.emitUint16(leftOpr);
    if (insn.rightFormat() == ADDR16 || insn.rightFormat() == IMM16)
        insn.emitUint16(rightOpr);
    return setError(OK);
}

Error AsmZ80::encodeIoaddr(
    Insn &insn, target::uint16_t leftOpr, target::uint16_t rightOpr) {
    emitInsnCode(insn);
    if (insn.leftFormat() == ADDR8)
        insn.emitByte(leftOpr);
    if (insn.rightFormat() == ADDR8)
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
    insn.emitByte(target::byte_t(delta));
    return setError(OK);
}

Error AsmZ80::encodeIndexed(
    Insn &insn, RegName leftReg, RegName rightReg,
    target::uintptr_t leftOpr, target::uintptr_t rightOpr) {
    if (insn.leftFormat() == IX_OFF)
        InsnTable::encodePrefixCode(insn, leftReg);
    if (insn.rightFormat() == IX_OFF)
        InsnTable::encodePrefixCode(insn, rightReg);

    target::byte_t regNum = 0;
    switch (insn.insnFormat()) {
    case NO_FMT:
        break;
    case DST_FMT:
        regNum = Registers::encodeDataReg(leftReg) << 3;
        break;
    case SRC_FMT:
        regNum = Registers::encodeDataReg(rightReg);
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
    if (insn.rightFormat() == IMM8)
        insn.emitByte(rightOpr);
    return setError(OK);
}

Error AsmZ80::encodeIndexedImmediate8(
    Insn &insn, RegName leftReg, RegName rightReg,
    target::uintptr_t leftOpr, target::uintptr_t rightOpr) {
    const target::opcode_t prefixCode = InsnTable::prefixCode(insn.insnCode());
    target::opcode_t opc = InsnTable::opCode(insn.insnCode());
    insn.setInsnCode(prefixCode);
    if (insn.leftFormat() == IX_OFF)
        InsnTable::encodePrefixCode(insn, leftReg);
    if (insn.rightFormat() == IX_OFF)
        InsnTable::encodePrefixCode(insn, rightReg);
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
    Insn &insn, RegName leftReg, RegName rightReg, target::uint16_t leftOpr) {
    target::byte_t regNum = 0;
    switch (insn.insnFormat()) {
    case NO_FMT:
        if (insn.leftFormat() == IX_REG || insn.leftFormat() == IX_PTR)
            InsnTable::encodePrefixCode(insn, leftReg);
        if (insn.rightFormat() == IX_REG)
            InsnTable::encodePrefixCode(insn, rightReg);
        break;
    case DST_SRC_FMT:
        if (insn.leftFormat() == REG8 || insn.leftFormat() == HL_PTR) {
            regNum = Registers::encodeDataReg(leftReg);
        } else if (insn.leftFormat() == BIT_NO) {
            if (leftOpr >= 8) return setError(ILLEGAL_OPERAND);
            regNum = leftOpr;
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        if (insn.rightFormat() == REG8 || insn.rightFormat() == HL_PTR) {
            regNum |= Registers::encodeDataReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        break;
    case DST_FMT:
        if (insn.leftFormat() == REG8) {
            regNum = Registers::encodeDataReg(leftReg);
        } else if (insn.rightFormat() == REG8) {
            regNum = Registers::encodeDataReg(rightReg);
        } else if (insn.leftFormat() == CC8) {
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
        if (insn.leftFormat() == REG8) {
            regNum = Registers::encodeDataReg(leftReg);
        } else if (insn.rightFormat() == REG8) {
            regNum = Registers::encodeDataReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_FMT:
        if (insn.leftFormat() == IR_REG) {
            regNum = Registers::encodeIrReg(leftReg);
        } else if (insn.rightFormat() == IR_REG) {
            regNum = Registers::encodeIrReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        break;
    case IDX_FMT:
        if (insn.leftFormat() == BC_PTR) {
            regNum = Registers::encodeIndexReg(leftReg);
        } else if (insn.rightFormat() == BC_PTR) {
            regNum = Registers::encodeIndexReg(rightReg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 4;
        break;
    case PTR_FMT:
        if (insn.leftFormat() == IX_REG)
            InsnTable::encodePrefixCode(insn, leftReg);
        if (insn.leftFormat() == STK16) {
            regNum = Registers::encodeStackReg(leftReg);
        } else if (insn.leftFormat() == REG16) {
            regNum = Registers::encodePointerReg(leftReg);
        } else if (insn.rightFormat() == REG16) {
            regNum = Registers::encodePointerReg(rightReg);
        } else if (insn.rightFormat() == REG16X) {
            regNum = Registers::encodePointerRegIx(rightReg, leftReg);
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

const char *AsmZ80::parseOperand(
    const char *line, OprFormat &oprFormat,
    RegName &regName, target::uint16_t &operand) {
    setError(OK);

    if (oprFormat == CC4 || oprFormat == CC8) {
        target::int8_t cc;
        line = Registers::parseCc4Name(line, cc);
        if (cc >= 0) {
            oprFormat = CC4;
            operand = cc;
            return line;
        }
        line = Registers::parseCc8Name(line, cc);
        if (cc >= 0) {
            oprFormat = CC8;
            operand = cc;
            return line;
        }
    }

    regName = NONE;
    operand = 0;
    if (*line == 0) {
        oprFormat = NO_OPR;
        return line;
    }

    regName = Registers::parseRegister(line);
    if (regName != NONE) {
        line += Registers::regNameLen(regName);
        switch (regName) {
        case A:   oprFormat = A_REG; break;
        case BC:  oprFormat = BC_REG; break;
        case DE:  oprFormat = DE_REG; break;
        case HL:  oprFormat = HL_REG; break;
        case SP:  oprFormat = SP_REG; break;
        case IX:
        case IY:  oprFormat = IX_REG; break;
        case I:
        case R:   oprFormat = IR_REG; break;
        case AF:  oprFormat = AF_REG; break;
        case AFP: oprFormat = AFPREG; break;
        default:  oprFormat = REG8; break;
        }
        return line;
    } else if (*line == '(') {
        regName = Registers::parseRegister(++line);
        if (regName == NONE) {
            if (getOperand16(line, operand) == OK && *line == ')') {
                line++;
                oprFormat = (operand < 0x100) ? ADDR8 : ADDR16;
            } else setError(UNKNOWN_OPERAND);
            return line;
        }
        line += Registers::regNameLen(regName);
        if (*line == ')') {
            line++;
            switch (regName) {
            case BC:
            case DE: oprFormat = BC_PTR; break;
            case HL: oprFormat = HL_PTR; break;
            case SP: oprFormat = SP_PTR; break;
            case IX:
            case IY: oprFormat = IX_PTR; break;
            case C:  oprFormat = C_PTR; break;
            default: setError(UNKNOWN_OPERAND);
            }
            return line;
        } else if (*line == '+' || *line == '-') {
            if ((regName == IX || regName == IY)
                && getOperand16(line, operand) == OK && *line == ')') {
                line++;
                const target::int16_t offset = target::int16_t(operand);
                if (offset >= -128 && offset < 128) {
                    oprFormat = IX_OFF;
                    return line;
                }
            }
        }
        setError(UNKNOWN_OPERAND);
        return line;
    }
    if (getOperand16(line, operand) == OK) {
        if (operand < 0x100) oprFormat = IMM8;
        else oprFormat = IMM16;
        return line;
    }
    setError(UNKNOWN_OPERAND);
    return line;
}

Error AsmZ80::encode(
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

    if (InsnTable.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);

    OprFormat leftFormat = insn.leftFormat();
    OprFormat rightFormat = insn.rightFormat();
    RegName leftReg, rightReg;
    target::uint16_t leftOpr, rightOpr;
    line = parseOperand(line, leftFormat, leftReg, leftOpr);
    if (getError()) return getError();
    if (*line == ',') {
        line = skipSpace(++line);
        line = parseOperand(line, rightFormat, rightReg, rightOpr);
        if (getError()) return getError();
    } else {
        rightFormat = NO_OPR;
        rightReg = NONE;
        rightOpr = 0;
    }

    if (InsnTable.searchNameAndOprFormats(insn, leftFormat, rightFormat))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHERENT:
        return encodeInherent(insn, leftReg, rightReg, leftOpr);
    case IMMEDIATE8:
    case IMMEDIATE16:
        return encodeImmediate(insn, leftReg, rightOpr);
    case DIRECT:
        return encodeDirect(insn, leftReg, rightReg, leftOpr, rightOpr);
    case IOADDR:
        return encodeIoaddr(insn, leftOpr, rightOpr);
    case RELATIVE:
        return encodeRelative(insn, leftOpr, rightOpr);
    case INDEXED:
    case INDEXED_IMMEDIATE8:
        if (insn.addrMode() == INDEXED || insn.rightFormat() == IMM8) {
            return encodeIndexed(insn, leftReg, rightReg, leftOpr, rightOpr);
        } else {
            return encodeIndexedImmediate8(insn, leftReg, rightReg, leftOpr, rightOpr);
        }
    default: break;
    }

    return setError(UNKNOWN_INSTRUCTION);
}
