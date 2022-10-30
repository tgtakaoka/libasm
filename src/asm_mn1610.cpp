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

namespace libasm {
namespace mn1610 {

Error AsmMn1610::encodeIcRelative(InsnMn1610 &insn, const Operand &op) {
    if (checkAddress(op.val32, op))
        return getError();
    const int32_t delta = op.val32 - insn.address();
    if (overflowRel8(delta))
        return setError(op, OPERAND_TOO_FAR);
    const Config::opcode_t mode = (op.mode == M_IABS) ? 3 : 1;
    insn.embed(mode << 11);
    insn.embed(static_cast<uint8_t>(delta));
    return OK;
}

Error AsmMn1610::encodeGenericAddress(InsnMn1610 &insn, const Operand &op) {
    switch (op.mode) {
    case M_IABS:
        if (op.val32 < 0x100) {  // Zero-page indirect: (D)
            insn.embed(2 << 11);
            insn.embed(static_cast<uint8_t>(op.val32));
            return OK;
        }
        // Relative indirect: (abs)
        // Fall-through
    case M_ABS:
    case M_IM16:  // Relative: abs
        return encodeIcRelative(insn, op);
    case M_IXID:
    case M_INDX:
    case M_IDIX:
        if (RegMn1610::isIndex(op.reg)) {
            if (op.mode == M_IXID)
                return setError(op, REGISTER_NOT_ALLOWED);
            if (op.val32 >= 0x100)
                return setError(op, OVERFLOW_RANGE);
            // Direct index: D(Xj), Indirect index: (D)(Xj)
            insn.embed(static_cast<uint8_t>(op.val32));
            const Config::opcode_t mode =
                    (op.mode == M_IDIX ? 2 : 0) | RegMn1610::encodeIndex(op.reg);
            insn.embed(mode << 11);
            return OK;
        }
        if (op.reg == REG_IC) {
            if (op.mode == M_IDIX)
                return setError(op, REGISTER_NOT_ALLOWED);
            if (overflowRel8(static_cast<int32_t>(op.val32)))
                return setError(op, OPERAND_TOO_FAR);
            // Relative: d(IC), Relative indirect: (d(IC))
            insn.embed(static_cast<uint8_t>(op.val32));
            insn.embed((op.mode == M_INDX ? 1 : 3) << 11);
            return OK;
        }
        return setError(op, REGISTER_NOT_ALLOWED);
    case M_IM8:
    case M_IM4:
    case M_ILVL:
        // Zero-page direct: D
        insn.embed((0 << 11) | op.val32);
        return OK;
    default:
        return setError(op, UNKNOWN_OPERAND);
    }
}

Error AsmMn1610::encodeOperand(InsnMn1610 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case M_RDG:
        if (op.reg == REG_STR)
            return setError(op, REGISTER_NOT_ALLOWED);
        // Fall-Through
    case M_RD:
        insn.embed(RegMn1610::encodeGeneric(op.reg) << 8);
        break;
    case M_RSG:
        if (op.reg == REG_STR)
            return setError(op, REGISTER_NOT_ALLOWED);
        // Fall-Through
    case M_RS:
        insn.embed(RegMn1610::encodeGeneric(op.reg));
        break;
    case M_RBW:
        if (op.reg == REG_CSBR)
            return setError(op, REGISTER_NOT_ALLOWED);
        // Fall-Through
    case M_SB:
    case M_RB:
        insn.embed(RegMn1610::encodeSegment(op.reg) << 4);
        return OK;
    case M_RHR:
    case M_RHW:
        if ((mode == M_RHR && op.reg == REG_SOR) || (mode == M_RHW && op.reg == REG_SIR))
            return setError(op, REGISTER_NOT_ALLOWED);
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
            return setError(op, ILLEGAL_CONSTANT);
        insn.embed((op.val32 & 1) << 3);
        return OK;
    case M_IM8W:
    case M_IM16:
        if (overflowUint16(op.val32))
            return setError(op, OVERFLOW_RANGE);
        // Fall-through
    case M_IABS:
        insn.emitOperand16(op.val32);
        return OK;
    case M_BIT:
        if (op.val32 >= 16)
            return setError(op, ILLEGAL_BIT_NUMBER);
        insn.embed(static_cast<uint8_t>(op.val32));
        return OK;
    case M_ILVL:
        if (op.val32 >= 4)
            return setError(op, ILLEGAL_CONSTANT);
        // Fall-through
    case M_IM4:
        if (op.val32 >= 16)
            return setError(op, OVERFLOW_RANGE);
        // Fall-through
    case M_IM8:
    case M_IOA:
        if (overflowUint8(op.val32))
            return setError(op, OVERFLOW_RANGE);
        insn.embed(static_cast<uint8_t>(op.val32));
        return OK;
    case M_ABS:
        // TODO: calculate/check segment/offset
        if (checkAddress(op.val32, op))
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
        return setError(op, UNKNOWN_OPERAND);
    }
    return OK;
}

Error AsmMn1610::parseOperand(StrScanner &scan, Operand &op) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    op.cc = RegMn1610::parseCcName(p);
    if (op.cc != CC_UNDEF) {
        if (RegMn1610::isSkip(op.cc)) {
            op.mode = M_SKIP;
        } else if (RegMn1610::isCop(op.cc)) {
            op.mode = M_COP;
        } else if (RegMn1610::isEop(op.cc)) {
            op.mode = M_EOP;
        }
        scan = p;
        return OK;
    }

    StrScanner t(p);
    const bool indir = p.expect('(');
    const bool preDec = (*t == '-' && *++t == '(');
    if (indir || preDec)
        ++t;
    const StrScanner r(t);
    op.reg = RegMn1610::parseRegName(t);
    if (op.reg != REG_UNDEF) {
        // r, (r), -(r), (r)+
        t.skipSpaces();
        if (indir || preDec) {
            if (op.reg == REG_R1) {
                op.mode = M_RI1;
            } else if (op.reg == REG_R2) {
                op.mode = M_RI2;
            } else if (RegMn1610::isIndirect(op.reg)) {
                op.mode = M_RI;
            } else {
                return op.setError(r, REGISTER_NOT_ALLOWED);
            }
            if (!t.expect(')'))
                return op.setError(t, MISSING_CLOSING_PAREN);
            if (preDec) {
                op.mode = M_RIAU;
                op.val32 = -1;
            } else if (t.expect('+')) {
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
        scan = t;
        return OK;
    }

    // v, (v), (v)(r), v(r), (v(r))
    op.val32 = parseExpr32(p, op);
    if (parserError())
        return getError();
    if (endOfLine(*p.skipSpaces()) || *p == ',') {
        // v
        if (overflowUint16(op.val32)) {
            op.mode = M_ABS;
        } else if (overflowUint8(op.val32)) {
            op.mode = M_IM16;
        } else {
            op.mode = M_IM8;
        }
        scan = p;
        return OK;
    }
    if (p.expect('(')) {
        // v(r), (v(r))
        op.reg = RegMn1610::parseRegName(p.skipSpaces());
        if (op.reg == REG_UNDEF)
            return op.setError(p, UNKNOWN_OPERAND);
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        p.skipSpaces();
        if (indir && !p.expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        op.mode = indir ? M_IXID : M_INDX;
        scan = p;
        return OK;
    }
    if (indir && !p.expect(')'))
        return op.setError(p, MISSING_CLOSING_PAREN);
    // (v), (v)(r)
    if (endOfLine(*p.skipSpaces()) || *p == ',') {
        op.mode = M_IABS;
        scan = p;
        return OK;
    }
    if (p.expect('(')) {
        // (v)(r)
        op.reg = RegMn1610::parseRegName(p.skipSpaces());
        if (op.reg != REG_UNDEF) {
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = M_IDIX;
            scan = p;
            return OK;
        }
    }
    return op.setError(UNKNOWN_OPERAND);
}

Error AsmMn1610::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnMn1610 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    Operand op1, op2, op3, op4;
    if (parseOperand(scan, op1) && op1.hasError())
        return setError(op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2) && op2.hasError())
            return setError(op2);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, op3) && op3.hasError())
            return setError(op3);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, op4) && op4.hasError())
            return setError(op4);
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);
    setErrorIf(op3);
    setErrorIf(op4);

    insn.setAddrMode(op1.mode, op2.mode, op3.mode, op4.mode);
    const auto error = TableMn1610::TABLE.searchName(insn);
    if (error)
        return setError(op1, error);

    const AddrMode mode1 = insn.mode1();
    if (mode1 != M_NONE && encodeOperand(insn, op1, mode1)) {
    error:
        insn.reset();
        return getError();
    }
    const AddrMode mode2 = insn.mode2();
    if (mode2 != M_NONE && encodeOperand(insn, op2, mode2))
        goto error;
    const AddrMode mode3 = insn.mode3();
    if (mode3 != M_NONE && encodeOperand(insn, op3, mode3))
        goto error;
    const AddrMode mode4 = insn.mode4();
    if (mode4 != M_NONE && encodeOperand(insn, op4, mode4))
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
