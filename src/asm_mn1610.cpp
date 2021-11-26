/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "asm_mn1610.h"

#include "table_mn1610.h"

namespace libasm {
namespace mn1610 {

Error AsmMn1610::checkAddressRange(Config::uintptr_t addr) {
    return TableMn1610.checkAddressRange(addr);
}

Error AsmMn1610::encodeIcRelative(InsnMn1610 &insn, const Operand &op) {
    if (checkAddressRange(op.val32))
        return getError();
    const int32_t delta = op.val32 - insn.address();
    if (overflowRel8(delta))
        return setError(OPERAND_TOO_FAR);
    const Config::opcode_t mode = (op.mode == M_IABS) ? 3 : 1;
    insn.embed(mode << 11);
    insn.embed(static_cast<uint8_t>(delta));
    return OK;
}

Error AsmMn1610::encodeGenericAddress(InsnMn1610 &insn, const Operand &op) {
    switch (op.mode) {
    case M_IABS:
        if (op.val32 < 0x100) { // Zero-page indirect: (D)
            insn.embed(2 << 11);
            insn.embed(static_cast<uint8_t>(op.val32));
            return OK;
        }
        // Relative indirect: (abs)
        // Fall-through
    case M_ABS:
    case M_IM16: // Relative: abs
        return encodeIcRelative(insn, op);
    case M_IXID:
    case M_INDX:
    case M_IDIX:
        if (RegMn1610::isIndex(op.reg)) {
            if (op.mode == M_IXID)
                return setError(REGISTER_NOT_ALLOWED);
            if (op.val32 >= 0x100)
                return setError(OVERFLOW_RANGE);
            // Direct index: D(Xj), Indirect index: (D)(Xj)
            insn.embed(static_cast<uint8_t>(op.val32));
            const Config::opcode_t mode =
                    (op.mode == M_IDIX ? 2 : 0) | RegMn1610::encodeIndex(op.reg);
            insn.embed(mode << 11);
            return OK;
        }
        if (op.reg == REG_IC) {
            if (op.mode == M_IDIX)
                return setError(REGISTER_NOT_ALLOWED);
            if (overflowRel8(static_cast<int32_t>(op.val32)))
                return setError(OPERAND_TOO_FAR);
            // Relative: d(IC), Relative indirect: (d(IC))
            insn.embed(static_cast<uint8_t>(op.val32));
            insn.embed((op.mode == M_INDX ? 1 : 3) << 11);
            return OK;
        }
        return setError(REGISTER_NOT_ALLOWED);
    case M_IM8:
    case M_IM4:
    case M_ILVL:
        // Zero-page direct: D
        insn.embed((0 << 11) | op.val32);
        return OK;
    default:
        return setError(UNKNOWN_OPERAND);
    }
}

Error AsmMn1610::encodeOperand(InsnMn1610 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case M_RDG:
        if (op.reg == REG_STR)
            return setError(REGISTER_NOT_ALLOWED);
        // Fall-Through
    case M_RD:
        insn.embed(RegMn1610::encodeGeneric(op.reg) << 8);
        break;
    case M_RSG:
        if (op.reg == REG_STR)
            return setError(REGISTER_NOT_ALLOWED);
        // Fall-Through
    case M_RS:
        insn.embed(RegMn1610::encodeGeneric(op.reg));
        break;
    case M_RBW:
        if (op.reg == REG_CSBR)
            return setError(REGISTER_NOT_ALLOWED);
        // Fall-Through
    case M_SB:
    case M_RB:
        insn.embed(RegMn1610::encodeSegment(op.reg) << 4);
        return OK;
    case M_RHR:
    case M_RHW:
        if ((mode == M_RHR && op.reg == REG_SOR) || (mode == M_RHW && op.reg == REG_SIR))
            return setError(REGISTER_NOT_ALLOWED);
        insn.embed(RegMn1610::encodeHardware(op.reg) << 4);
        return OK;
    case M_RP:
        insn.embed(RegMn1610::encodeSpecial(op.reg) << 4);
        return OK;
    case M_RIAU:
        insn.embed((op.mode == M_RIAU ? (op.val32 == 1 ? 3 : 2) : 1) << 6);
        // Fall-through
    case M_RI:
        insn.embed(RegMn1610::encodeIndirect(op.reg));
        return OK;
    case M_SKIP:
        if (op.mode == M_SKIP)  // op.mode may be M_NONE
            insn.embed(RegMn1610::encodeSkip(op.cc) << 4);
        return OK;
    case M_EOP:
        insn.embed(RegMn1610::encodeEop(op.cc));
        return OK;
    case M_COP:
        if (op.mode == M_COP) {
            insn.embed(RegMn1610::encodeCop(op.cc) << 3);
            return OK;
        }
        if (op.val32 >= 2)
            return setError(ILLEGAL_CONSTANT);
        insn.embed((op.val32 & 1) << 3);
        return OK;
    case M_IM8W:
    case M_IM16:
        if (overflowUint16(op.val32))
            return setError(OVERFLOW_RANGE);
        // Fall-through
    case M_IABS:
        insn.emitOperand16(op.val32);
        return OK;
    case M_BIT:
        if (op.val32 >= 16)
            return setError(ILLEGAL_BIT_NUMBER);
        insn.embed(static_cast<uint8_t>(op.val32));
        return OK;
    case M_ILVL:
        if (op.val32 >= 4)
            return setError(ILLEGAL_CONSTANT);
        // Fall-through
    case M_IM4:
        if (op.val32 >= 16)
            return setError(OVERFLOW_RANGE);
        // Fall-through
    case M_IM8:
    case M_IOA:
        if (overflowUint8(op.val32))
            return setError(OVERFLOW_RANGE);
        insn.embed(static_cast<uint8_t>(op.val32));
        return OK;
    case M_ABS:
        // TODO: calculate/check segment/offset
        if (checkAddressRange(op.val32))
            return getError();
        insn.emitOperand16(op.val32);
        return OK;
    case M_GEN:
        return encodeGenericAddress(insn, op);
    case M_R0:
    case M_DR0:
    case M_RI1:
    case M_RI2:
        return OK;
    default:
        return setError(UNKNOWN_OPERAND);
    }
    return OK;
}

Error AsmMn1610::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    op.cc = RegMn1610::parseCcName(p);
    if (op.cc != CC_UNDEF) {
        p += RegMn1610::ccNameLen(op.cc);
        if (RegMn1610::isSkip(op.cc)) {
            op.mode = M_SKIP;
        } else if (RegMn1610::isCop(op.cc)) {
            op.mode = M_COP;
        } else if (RegMn1610::isEop(op.cc)) {
            op.mode = M_EOP;
        }
        _scan = p;
        return OK;
    }

    const bool indir = (*p == '(');
    const bool preDec = (*p == '-' && p[1] == '(');
    const char *save = p;
    if (indir || preDec)
        p = skipSpaces(p + (preDec ? 2 : 1));
    op.reg = RegMn1610::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        // r, (r), -(r), (r)+
        p = skipSpaces(p + RegMn1610::regNameLen(op.reg));
        if (indir || preDec) {
            if (op.reg == REG_R1) {
                op.mode = M_RI1;
            } else if (op.reg == REG_R2) {
                op.mode = M_RI2;
            } else if (RegMn1610::isIndirect(op.reg)) {
                op.mode = M_RI;
            } else {
                return setError(REGISTER_NOT_ALLOWED);
            }
            if (*p++ != ')')
                return setError(MISSING_CLOSING_PAREN);
            if (preDec) {
                op.mode = M_RIAU;
                op.val32 = -1;
            } else if (*p == '+') {
                p++;
                op.mode = M_RIAU;
                op.val32 = 1;
            }
        } else {
            if (op.reg == REG_R0) {
                op.mode = M_R0;
            } else if (RegMn1610::isGeneric(op.reg)) {
                op.mode = (op.reg == REG_STR) ? M_RD : M_RDG;
            } else if (RegMn1610::isSegmentBase(op.reg)) {
                op.mode = M_SB;
            } else if (op.reg == REG_DR0) {
                op.mode = M_DR0;
            } else if (RegMn1610::isSegment(op.reg)) {
                op.mode = M_RB;
            } else if (RegMn1610::isSpecial(op.reg)) {
                op.mode = M_RP;
            } else if (RegMn1610::isHardware(op.reg)) {
                op.mode = M_RHR;
            }
        }
        _scan = p;
        return OK;
    }

    // v, (v), (v)(r), v(r), (v(r))
    p = indir ? save + 1 : save;
    op.val32 = parseExpr32(p);
    if (parserError())
        return getError();
    p = skipSpaces(_scan);
    if (endOfLine(p) || *p == ',') {
        // v
        if (overflowUint16(op.val32)) {
            op.mode = M_ABS;
        } else if (overflowUint8(op.val32)) {
            op.mode = M_IM16;
        } else {
            op.mode = M_IM8;
        }
        _scan = p;
        return OK;
    }
    if (*p == '(') {
        // v(r), (v(r))
        p = skipSpaces(p + 1);
        op.reg = RegMn1610::parseRegName(p);
        if (op.reg == REG_UNDEF)
            return setError(UNKNOWN_OPERAND);
        p = skipSpaces(p + RegMn1610::regNameLen(op.reg));
        if (*p++ != ')')
            return setError(MISSING_CLOSING_PAREN);
        p = skipSpaces(p);
        if (indir && *p++ != ')')
            return setError(MISSING_CLOSING_PAREN);
        _scan = p;
        op.mode = indir ? M_IXID : M_INDX;
        return OK;
    }
    if (indir && *p++ == ')') {
        // (v), (v)(r)
        p = skipSpaces(p);
        if (endOfLine(p) || *p == ',') {
            op.mode = M_IABS;
            _scan = p;
            return OK;
        }
        if (*p++ == '(') {
            // (v)(r)
            p = skipSpaces(p);
            op.reg = RegMn1610::parseRegName(p);
            if (op.reg != REG_UNDEF) {
                p = skipSpaces(p + RegMn1610::regNameLen(op.reg));
                if (*p++ == ')') {
                    _scan = p;
                    op.mode = M_IDIX;
                    return OK;
                }
            }
        }
        // Fall-throgh to MISSING_CLOSING_PAREN error
    }
    return setError(indir ? MISSING_CLOSING_PAREN : UNKNOWN_OPERAND);
}

Error AsmMn1610::encode(Insn &_insn) {
    InsnMn1610 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand opr1, opr2, opr3, opr4;
    if (parseOperand(endName, opr1))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, opr2))
            return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, opr3))
            return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, opr4))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(opr1.getError());
    setErrorIf(opr2.getError());
    setErrorIf(opr3.getError());
    setErrorIf(opr4.getError());

    insn.setAddrMode(opr1.mode, opr2.mode, opr3.mode, opr4.mode);
    if (TableMn1610.searchName(insn))
        return setError(TableMn1610.getError());

    const AddrMode op1 = insn.op1();
    if (op1 != M_NO && encodeOperand(insn, opr1, op1)) {
    error:
        insn.reset();
        return getError();
    }
    const AddrMode op2 = insn.op2();
    if (op2 != M_NO && encodeOperand(insn, opr2, op2))
        goto error;
    const AddrMode op3 = insn.op3();
    if (op3 != M_NO && encodeOperand(insn, opr3, op3))
        goto error;
    const AddrMode op4 = insn.op4();
    if (op4 != M_NO && encodeOperand(insn, opr4, op4))
        goto error;

    insn.emitInsn();
    return getError();
}

}  // namespace mn1610
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
