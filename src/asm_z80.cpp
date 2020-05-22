/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "asm_z80.h"
#include "table_z80.h"
#include "reg_z80.h"

namespace libasm {
namespace z80 {

Error AsmZ80::encodeImmediate(
    InsnZ80 &insn, const Operand &left, const Operand &right) {
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case DST_FMT:
        regNum = RegZ80::encodeDataReg(left.reg) << 3;
        break;
    case PTR_FMT:
        regNum = RegZ80::encodePointerReg(left.reg) << 4;
        break;
    case NO_FMT:
        if (insn.leftFormat() == IX_REG)
            RegZ80::encodeIndexReg(insn, left.reg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.embed(regNum);
    insn.emitInsn();
    if (insn.addrMode() == IMM8)
        insn.emitByte(right.val);
    if (insn.addrMode() == IMM16)
        insn.emitUint16(right.val);
    return checkLineEnd();
}

Error AsmZ80::encodeDirect(
    InsnZ80 &insn, const Operand &left, const Operand &right) {
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case DST_FMT:
        if (left.format == COND_8) regNum = left.val << 3;
        else return setError(INTERNAL_ERROR);
        break;
    case PTR_FMT:
        if (left.format == REG_16) {
            regNum = RegZ80::encodePointerReg(left.reg) << 4;
        } else if (right.format == REG_16) {
            regNum = RegZ80::encodePointerReg(right.reg) << 4;
        } else return setError(INTERNAL_ERROR);
        break;
    case NO_FMT:
        if (left.format == IX_REG)
            RegZ80::encodeIndexReg(insn, left.reg);
        if (right.format == IX_REG)
            RegZ80::encodeIndexReg(insn, right.reg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.embed(regNum);
    insn.emitInsn();
    if (left.format == ADDR_16 || left.format == IMM_16)
        insn.emitUint16(left.val);
    if (right.format == ADDR_16 || right.format == IMM_16)
        insn.emitUint16(right.val);
    return checkLineEnd();
}

Error AsmZ80::encodeIoaddr(
    InsnZ80 &insn, const Operand &left, const Operand &right) {
    insn.emitInsn();
    if (left.format == ADDR_8)
        insn.emitByte(left.val);
    if (right.format == ADDR_8)
        insn.emitByte(right.val);
    return checkLineEnd();
}

Error AsmZ80::encodeRelative(
    InsnZ80 &insn, const Operand &left, const Operand &right) {
    Config::uintptr_t addr = left.val;
    if (left.getError() == UNDEFINED_SYMBOL) addr = insn.address();
    if (insn.insnFormat() == CC4_FMT) {
        insn.embed(left.val << 3);
        addr = right.val;
        if (right.getError() == UNDEFINED_SYMBOL) addr = insn.address();
    }
    const Config::ptrdiff_t delta = addr - insn.address() - 2;
    if (delta < -128 || delta >= 128) return setError(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(delta));
    return checkLineEnd();
}

Error AsmZ80::encodeIndexed(
    InsnZ80 &insn, const Operand &left, const Operand &right) {
    if (left.format == IX_OFF) {
        if (right.format == REG_8 && right.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (IX+n),(HL)
        RegZ80::encodeIndexReg(insn, left.reg);
    }
    if (right.format == IX_OFF) {
        if (left.format == REG_8 && left.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (HL),(IX+n)
        RegZ80::encodeIndexReg(insn, right.reg);
    }

    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case NO_FMT:
        break;
    case DST_FMT:
        regNum = RegZ80::encodeDataReg(left.reg) << 3;
        break;
    case SRC_FMT:
        regNum = RegZ80::encodeDataReg(right.reg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.embed(regNum);
    insn.emitInsn();
    if (left.format == IX_OFF)
        insn.emitByte(left.val);
    if (right.format == IX_OFF || right.format == IMM_8)
        insn.emitByte(right.val);
    return checkLineEnd();
}

Error AsmZ80::encodeIndexedImmediate8(
    InsnZ80 &insn, const Operand &left, const Operand &right) {
    Config::opcode_t opc = insn.opCode();
    insn.setInsnCode(0, insn.prefixCode());
    if (left.format == IX_OFF)
        RegZ80::encodeIndexReg(insn, left.reg);
    if (right.format == IX_OFF)
        RegZ80::encodeIndexReg(insn, right.reg);
    if (insn.insnFormat() == DST_FMT)
        opc |= left.val << 3;
    insn.emitInsn();
    if (left.format == IX_OFF)
        insn.emitByte(left.val);
    if (right.format == IX_OFF)
        insn.emitByte(right.val);
    insn.emitByte(opc);
    setErrorIf(left.getError());
    setErrorIf(right.getError());
    return checkLineEnd();
}

Error AsmZ80::encodeInherent(
    InsnZ80 &insn, const Operand &left, const Operand &right) {
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case NO_FMT:
        if (left.format == IX_REG || left.format == IX_PTR)
            RegZ80::encodeIndexReg(insn, left.reg);
        if (right.format == IX_REG)
            RegZ80::encodeIndexReg(insn, right.reg);
        if (left.format == IM_REG || right.format == IM_REG)
            regNum = 0;         // 8085: RIM/SIM => LD A,IM/IM,A
        break;
    case DST_SRC_FMT:
        if (left.format == REG_8 && left.reg == REG_HL
            && right.format == REG_8 && right.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (HL),(HL)
        if (left.format == REG_8 || left.format == HL_PTR) {
            regNum = RegZ80::encodeDataReg(left.reg);
        } else if (left.format == BIT_NO) {
            if (left.val >= 8) return setError(ILLEGAL_OPERAND);
            regNum = left.val;
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        if (right.format == REG_8 || right.format == HL_PTR) {
            regNum |= RegZ80::encodeDataReg(right.reg);
        } else return setError(INTERNAL_ERROR);
        break;
    case DST_FMT:
        if (left.format == C_PTR && right.format == REG_8 && right.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (C),(HL)
        if (right.format == C_PTR && left.format == REG_8 && left.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (HL),(C)
        if (left.format == REG_8) {
            regNum = RegZ80::encodeDataReg(left.reg);
        } else if (right.format == REG_8) {
            regNum = RegZ80::encodeDataReg(right.reg);
        } else if (left.format == COND_8) {
            regNum = left.val;
        } else if (left.format == IMM_NO) {
            if (left.val == 0) regNum = 0;
            else if (left.val == 1) regNum = 2;
            else if (left.val  == 2) regNum = 3;
            else return setError(ILLEGAL_OPERAND);
        } else if (left.format == VEC_NO) {
            if ((left.val & ~0x38) != 0) return setError(ILLEGAL_OPERAND);
            regNum = left.val >> 3;
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        break;
    case SRC_FMT:
        if (left.format == REG_8) {
            regNum = RegZ80::encodeDataReg(left.reg);
        } else if (right.format == REG_8) {
            regNum = RegZ80::encodeDataReg(right.reg);
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_FMT:
        if (left.format == IR_REG) {
            regNum = RegZ80::encodeIrReg(left.reg);
        } else if (right.format == IR_REG) {
            regNum = RegZ80::encodeIrReg(right.reg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        break;
    case IDX_FMT:
        if (left.format == BC_PTR) {
            regNum = RegZ80::encodeIndirectBase(left.reg);
        } else if (right.format == BC_PTR) {
            regNum = RegZ80::encodeIndirectBase(right.reg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 4;
        break;
    case PTR_FMT:
        if (left.format == IX_REG)
            RegZ80::encodeIndexReg(insn, left.reg);
        if (left.format == STK_16) {
            regNum = RegZ80::encodeStackReg(left.reg);
        } else if (left.format == REG_16) {
            regNum = RegZ80::encodePointerReg(left.reg);
        } else if (right.format == REG_16) {
            regNum = RegZ80::encodePointerReg(right.reg);
        } else if (right.format == REG_16X) {
            regNum = RegZ80::encodePointerRegIx(right.reg, left.reg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 4;
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.embed(regNum);
    insn.emitInsn();
    return checkLineEnd();
}

Error AsmZ80::parseOperand(const InsnZ80 &insn, Operand &opr) {
    opr.setOK();
    opr.reg = REG_UNDEF;
    opr.val = 0;

    const char *p = _scan;
    if (endOfLine(p)) {
        opr.format = NO_OPR;
        return opr.setOK();
    }

    if (opr.format == COND_4 || opr.format == COND_8) {
        CcName ccName;
        if ((ccName = _regs.parseCc4Name(p)) != CC_UNDEF) {
            _scan = p + RegZ80::ccNameLen(ccName);
            opr.format = COND_4;
            opr.val = RegZ80::encodeCcName(ccName);
            return opr.setOK();
        }
        if ((ccName = _regs.parseCc8Name(p)) != CC_UNDEF) {
            _scan = p + RegZ80::ccNameLen(ccName);
            opr.format = COND_8;
            opr.val = RegZ80::encodeCcName(ccName);
            return opr.setOK();
        }
    }

    opr.reg = _regs.parseRegister(p);
    if (opr.reg != REG_UNDEF) {
        _scan = p + RegZ80::regNameLen(opr.reg);
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
        case REG_IM:  opr.format = IM_REG; break;
        default:      opr.format = REG_8; break;
        }
        if (opr.size == SZ_NONE)
            opr.size = RegZ80::registerSize(opr.reg);
        return opr.setOK();
    }
    if (*p == '(') {
        p = skipSpaces(p + 1);
        opr.reg = _regs.parseRegister(p);
        if (opr.reg == REG_UNDEF) {
            _scan = p;
            if (insn.addrMode() == IOADR) {
                uint8_t val8;
                if (getOperand(val8)) return opr.setError(getError());
                opr.val = val8;
                opr.format = ADDR_8;
            } else {
                if (getOperand(opr.val)) return opr.setError(getError());
                opr.format = ADDR_16;
            }
            p = skipSpaces(_scan);
            if (*p != ')') return opr.setError(UNKNOWN_OPERAND);
            _scan = p + 1;
            return opr.setOK();
        }
        p = skipSpaces(p + RegZ80::regNameLen(opr.reg));
        if (*p == ')') {
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
            _scan = p + 1;
            return opr.setOK();
        }
        if (*p == '+' || *p == '-') {
            if (opr.reg == REG_IX || opr.reg == REG_IY) {
                uint8_t val8;
                _scan = p;
                if (getOperand(val8)) return opr.setError(getError());
                opr.setError(getError());
                p = skipSpaces(_scan);
                if (*p != ')') return opr.setError(UNKNOWN_OPERAND);
                opr.val = val8;
                opr.format = IX_OFF;
                if (opr.size == SZ_NONE) opr.size = SZ_BYTE;
                _scan = p + 1;
                return OK;
            }
        }
        return opr.setError(UNKNOWN_OPERAND);
    }
    _scan = p;
    if (opr.size == SZ_WORD
        || insn.addrMode() == REL8 || insn.addrMode() == DIRECT) {
        if (getOperand(opr.val)) return opr.setError(getError());
        opr.setError(getError());
        opr.format = IMM_16;
        return OK;;
    }
    uint8_t val8;
    if (getOperand(val8)) return opr.setError(getError());
    opr.setError(getError());
    opr.val = val8;
    opr.format = IMM_8;
    return OK;
}

Error AsmZ80::encode(Insn &_insn) {
    InsnZ80 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableZ80.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    Operand left, right;
    left.format = insn.leftFormat();
    left.size = SZ_NONE;
    if (parseOperand(insn, left))
        return setError(left);
    _scan = skipSpaces(_scan);
    if (*_scan == ',') {
        _scan = skipSpaces(_scan + 1);
        right.format = insn.rightFormat();
        right.size = left.size;
        if (parseOperand(insn, right))
            return setError(right);
    } else {
        right.setOK();
        right.format = NO_OPR;
        right.reg = REG_UNDEF;
        right.val = 0;
    }

    if (TableZ80.searchNameAndOprFormats(insn, left.format, right.format))
        return setError(UNKNOWN_INSTRUCTION);
    left.format = insn.leftFormat();
    right.format = insn.rightFormat();

    switch (insn.addrMode()) {
    case INHR: return encodeInherent(insn, left, right);
    case IMM8:
    case IMM16:
        if (left.format == IMM_8 && right.format == NO_OPR) {
            // SUB/AND/XOR/OR/CP immediate instruction
            right.val = left.val;
        }
        return encodeImmediate(insn, left, right);
    case DIRECT:
        return encodeDirect(insn, left, right);
    case IOADR:
        return encodeIoaddr(insn, left, right);
    case REL8:
        return encodeRelative(insn, left, right);
    case INDX:
    case INDX_IMM8:
        if (insn.addrMode() == INDX || insn.rightFormat() == IMM_8) {
            return encodeIndexed(insn, left, right);
        } else {
            return encodeIndexedImmediate8(insn, left, right);
        }
    default: break;
    }

    return setError(UNKNOWN_INSTRUCTION);
}

} // namespace z80
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
