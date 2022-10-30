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

#include "asm_tlcs90.h"

namespace libasm {
namespace tlcs90 {

Error AsmTlcs90::encodeRelative(InsnTlcs90 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::ptrdiff_t delta = target - base;
    if (mode == M_REL16) {
        insn.emitUint16(delta);
        return OK;
    }
    if (overflowRel8(delta))
        return setError(op, OPERAND_TOO_FAR);
    insn.emitByte(delta);
    return OK;
}

Error AsmTlcs90::encodeOperand(
        InsnTlcs90 &insn, AddrMode mode, const Operand &op, Config::opcode_t opc) {
    switch (mode) {
    case M_IMM8:
        if (overflowUint8(op.val16))
            return setError(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_DIR:
        insn.emitInsn(opc);
        insn.emitByte(op.val16);
        return OK;
    case M_IMM16:
    case M_EXT:
        insn.emitInsn(opc);
        insn.emitUint16(op.val16);
        return OK;
    case M_BIT:
        if (op.val16 >= 8)
            return setError(op, OVERFLOW_RANGE);
        insn.emitInsn(opc | (op.val16 & 7));
        return OK;
    case M_REL8:
    case M_REL16:
        insn.emitInsn(opc);
        return encodeRelative(insn, mode, op);
    case M_IND:
        insn.emitInsn(opc | RegTlcs90::encodeReg16(op.reg));
        return OK;
    case M_IDX:
        insn.emitInsn(opc | RegTlcs90::encodeIndexReg(op.reg));
        insn.emitByte(op.val16);
        return OK;
    case M_CC:
        insn.emitInsn(opc | RegTlcs90::encodeCcName(op.cc));
        return OK;
    case M_STACK:
        insn.emitInsn(opc | RegTlcs90::encodeStackReg(op.reg));
        return OK;
    case M_REG8:
        insn.emitInsn(opc | RegTlcs90::encodeReg8(op.reg));
        return OK;
    case M_REG16:
        insn.emitInsn(opc | RegTlcs90::encodeReg16(op.reg));
        return OK;
    case M_REGIX:
        insn.emitInsn(opc | RegTlcs90::encodeIndexReg(op.reg));
        return OK;
    case R_A:
    case R_HL:
        return OK;
    default:
        insn.emitInsn(opc);
        return OK;
    }
}

Error AsmTlcs90::parseOperand(StrScanner &scan, Operand &op) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    const CcName cc = RegTlcs90::parseCcName(p);
    if (cc != CC_UNDEF) {
        op.mode = M_CC;
        op.cc = cc;
        if (cc == CC_C) {
            // 'C' is either REG_C or CC_C
            op.mode = R_C;
            op.reg = REG_C;
        }
        scan = p;
        return OK;
    }

    RegName reg = RegTlcs90::parseRegName(p);
    if (reg != REG_UNDEF) {
        switch (reg) {
        case REG_IX:
        case REG_IY:
            op.mode = M_REGIX;
            break;
        case REG_B:
        case REG_D:
        case REG_E:
        case REG_H:
        case REG_L:
            op.mode = M_REG8;
            break;
        default:
            op.mode = AddrMode(int8_t(reg) + 16);
            break;
        }
        op.reg = reg;
        scan = p;
        return OK;
    }
    if (p.expect('(')) {
        const StrScanner regp = p.skipSpaces();
        reg = RegTlcs90::parseRegName(p);
        if (reg == REG_UNDEF) {  // (nnnn)
            op.val16 = parseExpr16(p, op);
            if (parserError())
                return op.getError();
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = op.getError() ? M_SYM : (op.val16 >= 0xFF00 ? M_DIR : M_EXT);
            scan = p;
            return OK;
        }
        op.reg = reg;
        if (p.skipSpaces().expect(')')) {  // (rr)
            if (RegTlcs90::isReg16(reg)) {
                op.mode = M_IND;
                scan = p;
                return OK;
            }
            return op.setError(regp, REGISTER_NOT_ALLOWED);
        }
        StrScanner a(p);
        if (reg == REG_HL && a.expect('+')) {
            const StrScanner idxp = a.skipSpaces();
            const RegName idx = RegTlcs90::parseRegName(a);
            if (idx != REG_UNDEF) {
                if (idx != REG_A)
                    return op.setError(idxp, REGISTER_NOT_ALLOWED);
                if (!a.skipSpaces().expect(')'))
                    return op.setError(a, MISSING_CLOSING_PAREN);
                scan = a;
                op.mode = M_BASE;  // (HL+A)
                return OK;
            }
        }
        if (*p == '+' || *p == '-') {  // (rr+n)
            op.val16 = parseExpr16(p, op);
            if (parserError())
                return op.getError();
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            if (RegTlcs90::isRegIndex(reg)) {
                scan = p;
                op.mode = M_IDX;
                return OK;
            }
            return op.setError(regp, REGISTER_NOT_ALLOWED);
        }
        return op.setError(UNKNOWN_OPERAND);
    }
    op.val16 = parseExpr16(p, op);
    if (parserError())
        return op.getError();
    op.mode = M_IMM16;
    scan = p;
    return OK;
}

Error AsmTlcs90::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnTlcs90 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    Operand dstOp, srcOp;
    if (parseOperand(scan, dstOp) && dstOp.getError() != UNDEFINED_SYMBOL)
        return setError(dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp) && srcOp.getError() != UNDEFINED_SYMBOL)
            return setError(srcOp);
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);

    insn.setAddrMode(dstOp.mode, srcOp.mode);
    const auto error = TableTlcs90::TABLE.searchName(insn);
    if (error)
        return setError(dstOp, error);

    const AddrMode pre = insn.pre();
    const AddrMode dst = insn.dst();
    const AddrMode src = insn.src();
    const Config::opcode_t prefix = insn.prefix();
    if (prefix) {
        insn.setEmitInsn();
        if (pre == M_DST && encodeOperand(insn, dst, dstOp, prefix))
            return getError();
        if (pre == M_SRC && encodeOperand(insn, src, srcOp, prefix))
            return getError();
        if (pre == M_NONE)
            insn.emitByte(prefix);
    }
    const Config::opcode_t opc = insn.opCode();
    insn.setEmitInsn();
    if (pre != M_DST && encodeOperand(insn, dst, dstOp, opc))
        return getError();
    if (pre != M_SRC && encodeOperand(insn, src, srcOp, opc))
        return getError();
    insn.emitInsn(opc);
    return getError();
}

}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
