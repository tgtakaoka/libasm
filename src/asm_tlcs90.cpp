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

#include "entry_tlcs90.h"
#include "error_reporter.h"
#include "insn_tlcs90.h"
#include "reg_tlcs90.h"
#include "table_tlcs90.h"

namespace libasm {
namespace tlcs90 {

const char *aMode(AddrMode mode) {
    switch (mode) {
    case M_NO:
        return "M_NO";
    case M_IMM8:
        return "M_IMM8";
    case M_IMM16:
        return "M_IMM16";
    case M_BIT:
        return "M_BIT";
    case M_EXT:
        return "M_EXT";
    case M_DIR:
        return "M_DIR";
    case M_REL8:
        return "M_REL8";
    case M_REL16:
        return "M_REL16";
    case M_IND:
        return "M_IND";
    case M_IDX:
        return "M_IDX";
    case M_BASE:
        return "M_BASE";
    case M_CC:
        return "M_CC";
    case M_STACK:
        return "M_STACK";
    case M_REG8:
        return "M_REG8";
    case M_REG16:
        return "M_REG16";
    case M_REGIX:
        return "M_REGIX";
    case R_BC:
        return "R_BC";
    case R_DE:
        return "R_DE";
    case R_HL:
        return "R_HL";
    case R_SP:
        return "R_SP";
    case R_AF:
        return "R_AF";
    case R_AFP:
        return "R_AFP";
    case R_C:
        return "R_C";
    case R_A:
        return "R_A";
    case M_SRC16:
        return "M_SRC16";
    case M_SRC:
        return "M_SRC";
    case M_DST:
        return "M_DST";
    default:
        return "M_UNKI";
    }
}

Error AsmTlcs90::encodeRelative(InsnTlcs90 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::ptrdiff_t delta = target - base;
    if (mode == M_REL16) {
        insn.emitUint16(delta);
        return OK;
    }
    if (delta < -128 || delta >= 128)
        return setError(OPERAND_TOO_FAR);
    insn.emitByte(delta);
    return OK;
}

Error AsmTlcs90::encodeOperand(
        InsnTlcs90 &insn, AddrMode mode, const Operand &op, Config::opcode_t opc) {
    switch (mode) {
    case M_IMM8:
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

Error AsmTlcs90::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    const CcName cc = RegTlcs90::parseCcName(p);
    if (cc != CC_UNDEF) {
        _scan = p + RegTlcs90::ccNameLen(cc);
        op.mode = M_CC;
        op.cc = cc;
        if (cc == CC_C) {
            // 'C' is either REG_C or CC_C
            op.mode = R_C;
            op.reg = REG_C;
        }
        return OK;
    }

    RegName reg = RegTlcs90::parseRegName(p);
    if (reg != REG_UNDEF) {
        _scan = p + RegTlcs90::regNameLen(reg);
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
        return OK;
    }
    if (*p == '(') {
        p = skipSpaces(p + 1);
        reg = RegTlcs90::parseRegName(p);
        if (reg == REG_UNDEF) {  // (nnnn)
            op.val16 = parseExpr16(p);
            if (parserError())
                return getError();
            op.setError(getError());
            p = skipSpaces(_scan);
            if (*p != ')')
                return setError(MISSING_CLOSING_PAREN);
            op.mode = op.getError() ? M_UNDEF : (op.val16 >= 0xFF00 ? M_DIR : M_EXT);
            _scan = p + 1;
            return OK;
        }
        p = skipSpaces(p + RegTlcs90::regNameLen(reg));
        op.reg = reg;
        if (*p == ')') {  // (rr)
            if (RegTlcs90::isReg16(reg)) {
                op.mode = M_IND;
                _scan = p + 1;
                return OK;
            }
            return setError(REGISTER_NOT_ALLOWED);
        }
        if (reg == REG_HL && *p == '+') {
            const char *a = skipSpaces(p + 1);
            const RegName idx = RegTlcs90::parseRegName(a);
            if (idx != REG_UNDEF) {
                if (idx != REG_A)
                    return setError(REGISTER_NOT_ALLOWED);
                a = skipSpaces(a + RegTlcs90::regNameLen(REG_A));
                if (*a != ')')
                    return setError(MISSING_CLOSING_PAREN);
                _scan = a + 1;
                op.mode = M_BASE;  // (HL+A)
                return OK;
            }
        }
        if (*p == '+' || *p == '-') {  // (rr+n)
            op.val16 = parseExpr16(p);
            if (parserError())
                return getError();
            op.setError(getError());
            p = skipSpaces(_scan);
            if (*p != ')')
                return setError(MISSING_CLOSING_PAREN);
            if (RegTlcs90::isRegIndex(reg)) {
                _scan = p + 1;
                op.mode = M_IDX;
                return OK;
            }
            return setError(REGISTER_NOT_ALLOWED);
        }
        return setError(UNKNOWN_OPERAND);
    }
    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    op.setError(getError());
    op.mode = M_IMM16;
    return OK;
}

Error AsmTlcs90::encode(Insn &_insn) {
    InsnTlcs90 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand dstOp, srcOp;
    if (parseOperand(endName, dstOp))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, srcOp))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(dstOp.getError());
    setErrorIf(srcOp.getError());

    insn.setAddrMode(dstOp.mode, srcOp.mode);
    if (TableTlcs90.searchName(insn))
        return setError(TableTlcs90.getError());

    const AddrMode pre = insn.preMode();
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const Config::opcode_t prefix = insn.prefix();
    if (prefix) {
        insn.setEmitInsn();
        if (pre == M_DST && encodeOperand(insn, dst, dstOp, prefix))
            return getError();
        if (pre == M_SRC && encodeOperand(insn, src, srcOp, prefix))
            return getError();
        if (pre == M_NO)
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
