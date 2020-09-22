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
    InsnZ80 &insn, const Operand &dst, const Operand &src) {
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case DST_FMT:
        regNum = RegZ80::encodeDataReg(dst.reg) << 3;
        break;
    case PTR_FMT:
        regNum = RegZ80::encodePointerReg(dst.reg) << 4;
        break;
    case NO_FMT:
        if (insn.dstFormat() == IX_REG)
            RegZ80::encodeIndexReg(insn, dst.reg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.embed(regNum);
    insn.emitInsn();
    if (insn.addrMode() == IMM8)
        insn.emitByte(src.val16);
    if (insn.addrMode() == IMM16)
        insn.emitUint16(src.val16);
    return checkLineEnd();
}

Error AsmZ80::encodeDirect(
    InsnZ80 &insn, const Operand &dst, const Operand &src) {
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case DST_FMT:
        if (dst.format == COND_8) regNum = dst.val16 << 3;
        else return setError(INTERNAL_ERROR);
        break;
    case PTR_FMT:
        if (dst.format == REG_16) {
            regNum = RegZ80::encodePointerReg(dst.reg) << 4;
        } else if (src.format == REG_16) {
            regNum = RegZ80::encodePointerReg(src.reg) << 4;
        } else return setError(INTERNAL_ERROR);
        break;
    case NO_FMT:
        if (dst.format == IX_REG)
            RegZ80::encodeIndexReg(insn, dst.reg);
        if (src.format == IX_REG)
            RegZ80::encodeIndexReg(insn, src.reg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.embed(regNum);
    insn.emitInsn();
    if (dst.format == ADDR_16 || dst.format == IMM_16)
        insn.emitUint16(dst.val16);
    if (src.format == ADDR_16 || src.format == IMM_16)
        insn.emitUint16(src.val16);
    return checkLineEnd();
}

Error AsmZ80::encodeIoaddr(
    InsnZ80 &insn, const Operand &dst, const Operand &src) {
    insn.emitInsn();
    if (dst.format == ADDR_8)
        insn.emitByte(dst.val16);
    if (src.format == ADDR_8)
        insn.emitByte(src.val16);
    return checkLineEnd();
}

Error AsmZ80::encodeRelative(
    InsnZ80 &insn, const Operand &dst, const Operand &src) {
    const Operand *t = &dst;
    if (insn.insnFormat() == CC4_FMT) {
        insn.embed(dst.val16 << 3);
        t = &src;
    }
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = t->getError() ? base : t->val16;
    const Config::ptrdiff_t delta = target - base;
    if (delta < -128 || delta >= 128) return setError(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitByte(delta);
    return checkLineEnd();
}

Error AsmZ80::encodeIndexed(
    InsnZ80 &insn, const Operand &dst, const Operand &src) {
    if (dst.format == IX_OFF) {
        if (src.format == REG_8 && src.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (IX+n),(HL)
        RegZ80::encodeIndexReg(insn, dst.reg);
    }
    if (src.format == IX_OFF) {
        if (dst.format == REG_8 && dst.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (HL),(IX+n)
        RegZ80::encodeIndexReg(insn, src.reg);
    }

    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case NO_FMT:
        break;
    case DST_FMT:
        regNum = RegZ80::encodeDataReg(dst.reg) << 3;
        break;
    case SRC_FMT:
        regNum = RegZ80::encodeDataReg(src.reg);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.embed(regNum);
    insn.emitInsn();
    if (dst.format == IX_OFF)
        insn.emitByte(dst.val16);
    if (src.format == IX_OFF || src.format == IMM_8)
        insn.emitByte(src.val16);
    return checkLineEnd();
}

Error AsmZ80::encodeIndexedImmediate8(
    InsnZ80 &insn, const Operand &dst, const Operand &src) {
    Config::opcode_t opc = insn.opCode();
    insn.setOpCode(insn.prefix());
    if (dst.format == IX_OFF)
        RegZ80::encodeIndexReg(insn, dst.reg);
    if (src.format == IX_OFF)
        RegZ80::encodeIndexReg(insn, src.reg);
    if (insn.insnFormat() == DST_FMT)
        opc |= dst.val16 << 3;
    insn.emitInsn();
    if (dst.format == IX_OFF)
        insn.emitByte(dst.val16);
    if (src.format == IX_OFF)
        insn.emitByte(src.val16);
    insn.emitByte(opc);
    setErrorIf(dst.getError());
    setErrorIf(src.getError());
    return checkLineEnd();
}

Error AsmZ80::encodeInherent(
    InsnZ80 &insn, const Operand &dst, const Operand &src) {
    uint8_t regNum = 0;
    switch (insn.insnFormat()) {
    case NO_FMT:
        if (dst.format == IX_REG || dst.format == IX_PTR)
            RegZ80::encodeIndexReg(insn, dst.reg);
        if (src.format == IX_REG)
            RegZ80::encodeIndexReg(insn, src.reg);
        if (dst.format == IM_REG || src.format == IM_REG)
            regNum = 0;         // 8085: RIM/SIM => LD A,IM/IM,A
        break;
    case DST_SRC_FMT:
        if (dst.format == REG_8 && dst.reg == REG_HL
            && src.format == REG_8 && src.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (HL),(HL)
        if (dst.format == REG_8 || dst.format == HL_PTR) {
            regNum = RegZ80::encodeDataReg(dst.reg);
        } else if (dst.format == BIT_NO) {
            if (dst.val16 >= 8) return setError(ILLEGAL_OPERAND);
            regNum = dst.val16;
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        if (src.format == REG_8 || src.format == HL_PTR) {
            regNum |= RegZ80::encodeDataReg(src.reg);
        } else return setError(INTERNAL_ERROR);
        break;
    case DST_FMT:
        if (dst.format == C_PTR && src.format == REG_8 && src.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (C),(HL)
        if (src.format == C_PTR && dst.format == REG_8 && dst.reg == REG_HL)
            return setError(ILLEGAL_OPERAND); // (HL),(C)
        if (dst.format == REG_8) {
            regNum = RegZ80::encodeDataReg(dst.reg);
        } else if (src.format == REG_8) {
            regNum = RegZ80::encodeDataReg(src.reg);
        } else if (dst.format == COND_8) {
            regNum = dst.val16;
        } else if (dst.format == IMM_NO) {
            if (dst.val16 == 0) regNum = 0;
            else if (dst.val16 == 1) regNum = 2;
            else if (dst.val16  == 2) regNum = 3;
            else return setError(ILLEGAL_OPERAND);
        } else if (dst.format == VEC_NO) {
            if ((dst.val16 & ~0x38) != 0) return setError(ILLEGAL_OPERAND);
            regNum = dst.val16 >> 3;
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        break;
    case SRC_FMT:
        if (dst.format == REG_8) {
            regNum = RegZ80::encodeDataReg(dst.reg);
        } else if (src.format == REG_8) {
            regNum = RegZ80::encodeDataReg(src.reg);
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_FMT:
        if (dst.format == IR_REG) {
            regNum = RegZ80::encodeIrReg(dst.reg);
        } else if (src.format == IR_REG) {
            regNum = RegZ80::encodeIrReg(src.reg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 3;
        break;
    case IDX_FMT:
        if (dst.format == BC_PTR) {
            regNum = RegZ80::encodeIndirectBase(dst.reg);
        } else if (src.format == BC_PTR) {
            regNum = RegZ80::encodeIndirectBase(src.reg);
        } else return setError(INTERNAL_ERROR);
        regNum <<= 4;
        break;
    case PTR_FMT:
        if (dst.format == IX_REG)
            RegZ80::encodeIndexReg(insn, dst.reg);
        if (dst.format == STK_16) {
            regNum = RegZ80::encodeStackReg(dst.reg);
        } else if (dst.format == REG_16) {
            regNum = RegZ80::encodePointerReg(dst.reg);
        } else if (src.format == REG_16) {
            regNum = RegZ80::encodePointerReg(src.reg);
        } else if (src.format == REG_16X) {
            regNum = RegZ80::encodePointerRegIx(src.reg, dst.reg);
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

Error AsmZ80::parseOperand(Operand &op) {
    const char *p = _scan;
    if (endOfLine(p)) {
        op.format = NO_OPR;
        return op.setOK();
    }

    // 'C' is either C-reg or C-condition
    const RegName reg = RegZ80::parseRegName(p);
    if (reg == REG_C) {
        _scan = p + RegZ80::regNameLen(REG_C);
        op.reg = REG_C;
        op.size = SZ_BYTE;
        op.val16 = RegZ80::encodeCcName(CC_C);
        op.format = C_REG;
        return op.setOK();
    }

    const CcName cc = RegZ80::parseCcName(p);
    if (cc != CC_UNDEF) {
        op.format = RegZ80::isCc4Name(cc) ? COND_4 : COND_8;
        op.val16 = RegZ80::encodeCcName(cc);
        _scan = p + RegZ80::ccNameLen(cc);
        return op.setOK();
    }

    op.reg = RegZ80::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        _scan = p + RegZ80::regNameLen(op.reg);
        switch (op.reg) {
        case REG_A:   op.format = A_REG; break;
        case REG_BC:  op.format = BC_REG; break;
        case REG_DE:  op.format = DE_REG; break;
        case REG_HL:  op.format = HL_REG; break;
        case REG_SP:  op.format = SP_REG; break;
        case REG_IX:
        case REG_IY:  op.format = IX_REG; break;
        case REG_I:
        case REG_R:   op.format = IR_REG; break;
        case REG_AF:  op.format = AF_REG; break;
        case REG_AFP: op.format = AFPREG; break;
        case REG_IM:  op.format = IM_REG; break;
        default:      op.format = REG_8; break;
        }
        if (op.size == SZ_NONE)
            op.size = RegZ80::regSize(op.reg);
        return op.setOK();
    }
    if (*p == '(') {
        p = skipSpaces(p + 1);
        op.reg = RegZ80::parseRegName(p);
        if (op.reg == REG_UNDEF) {
            op.val16 = parseExpr16(p);
            if (parserError()) return op.setError(getError());
            op.format = ADDR_16;
            p = skipSpaces(_scan);
            if (*p != ')') return op.setError(MISSING_CLOSING_PAREN);
            _scan = p + 1;
            return op.setOK();
        }
        p = skipSpaces(p + RegZ80::regNameLen(op.reg));
        if (*p == ')') {
            switch (op.reg) {
            case REG_BC:
            case REG_DE: op.format = BC_PTR; break;
            case REG_HL: op.format = HL_PTR; break;
            case REG_SP: op.format = SP_PTR; break;
            case REG_IX:
            case REG_IY: op.format = IX_PTR; break;
            case REG_C:  op.format = C_PTR; break;
            default:     return op.setError(UNKNOWN_OPERAND);
            }
            if (op.size == SZ_NONE) op.size = SZ_BYTE;
            _scan = p + 1;
            return op.setOK();
        }
        if (*p == '+' || *p == '-') {
            if (op.reg == REG_IX || op.reg == REG_IY) {
                op.val16 = parseExpr16(p);
                if (parserError()) return op.setError(getError());
                op.setError(getError());
                p = skipSpaces(_scan);
                if (*p != ')') return op.setError(MISSING_CLOSING_PAREN);
                _scan = p + 1;
                op.format = IX_OFF;
                if (op.size == SZ_NONE) op.size = SZ_BYTE;
                return OK;
            }
        }
        return op.setError(UNKNOWN_OPERAND);
    }
    op.val16 = parseExpr16(p);
    if (parserError()) return op.setError(getError());
    op.setError(getError());
    op.format = IMM_16;
    return OK;;
}

Error AsmZ80::encode(Insn &_insn) {
    InsnZ80 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    _scan = skipSpaces(endName);
    Operand dst, src;
    if (parseOperand(dst)) return setError(dst);
    _scan = skipSpaces(_scan);
    if (*_scan == ',') {
        _scan = skipSpaces(_scan + 1);
        src.size = dst.size;
        if (parseOperand(src))
            return setError(src);
    }

    insn.setOprFormats(dst.format, src.format);
    if (TableZ80.searchName(insn))
        return setError(TableZ80.getError());
    dst.format = insn.dstFormat();
    src.format = insn.srcFormat();

    switch (insn.addrMode()) {
    case INHR: return encodeInherent(insn, dst, src);
    case IMM8:
    case IMM16:
        if (dst.format == IMM_8 && src.format == NO_OPR) {
            // SUB/AND/XOR/OR/CP immediate instruction
            src.val16 = dst.val16;
        }
        return encodeImmediate(insn, dst, src);
    case DIRECT:
        return encodeDirect(insn, dst, src);
    case IOADR:
        return encodeIoaddr(insn, dst, src);
    case REL8:
        return encodeRelative(insn, dst, src);
    case INDX:
    case INDX_IMM8:
        if (insn.addrMode() == INDX || insn.srcFormat() == IMM_8) {
            return encodeIndexed(insn, dst, src);
        } else {
            return encodeIndexedImmediate8(insn, dst, src);
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
