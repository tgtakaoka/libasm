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

#include "asm_tms9900.h"

namespace libasm {
namespace tms9900 {

Error AsmTms9900::encodeRelative(InsnTms9900 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    if (target % 2)
        return setError(op, OPERAND_NOT_ALIGNED);
    const Config::ptrdiff_t delta = (target - base) >> 1;
    if (overflowRel8(delta))
        return setError(op, OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(delta));
    return OK;
}

Error AsmTms9900::encodeCruOffset(InsnTms9900 &insn, const Operand &op) {
    const int16_t offset = static_cast<int16_t>(op.val16);
    if (overflowRel16(offset))
        return setError(op, OVERFLOW_RANGE);
    insn.embed(static_cast<uint8_t>(offset));
    return OK;
}

Error AsmTms9900::encodeModeReg(InsnTms9900 &insn, const Operand &op, AddrMode mode) {
    if (mode == M_SRC2 && insn.dst() == M_BIT2 && op.mode == M_INCR)
        return setError(op, OPERAND_NOT_ALLOWED);
    Config::opcode_t opc = RegTms9900::encodeRegNumber(op.reg);
    switch (op.mode) {
    case M_REG:
        break;
    case M_IREG:
        opc |= (1 << 4);
        break;
    case M_INCR:
        opc |= (3 << 4);
        break;
    case M_SYBL:
        opc = (2 << 4);
        insn.emitOperand16(op.val16);
        break;
    default:  // M_INDX
        if (op.reg == REG_R0)
            return setError(op, REGISTER_NOT_ALLOWED);
        opc |= (2 << 4);
        insn.emitOperand16(op.val16);
        break;
    }
    switch (mode) {
    case M_SRC:
        insn.embed(opc);
        break;
    case M_SRC2:
        insn.embedPost(opc);
        break;
    case M_DST:
        insn.embed(opc << 6);
        break;
    default:
        insn.embedPost(opc << 6);
        break;
    }
    return OK;
}

Error AsmTms9900::encodeOperand(InsnTms9900 &insn, const Operand &op, AddrMode mode) {
    uint16_t val16 = op.val16;
    switch (mode) {
    case M_IMM:
        insn.emitOperand16(op.val16);
        return OK;
    case M_REG:
        insn.embed(RegTms9900::encodeRegNumber(op.reg));
        return OK;
    case M_DREG:
        insn.embed(RegTms9900::encodeRegNumber(op.reg) << 6);
        return OK;
    case M_DST2:
        insn.embedPost(0x4000);
        /* Fall-through */
    case M_SRC:
    case M_SRC2:
    case M_DST:
        return encodeModeReg(insn, op, mode);
    case M_REL:
        return encodeRelative(insn, op);
    case M_CRU:
        return encodeCruOffset(insn, op);
    case M_CNT:
        if (val16 == 0 && !op.getError())
            return setError(op, OPERAND_NOT_ALLOWED);
        if (val16 == 16)
            val16 = 0;
        /* Fall-through */
    case M_XOP:
        if (val16 >= 16)
            return setError(op, OVERFLOW_RANGE);
        insn.embed(val16 << 6);
        return OK;
    case M_SCNT:
        if (op.mode == M_REG) {
            if (op.reg == REG_R0)
                return OK;
            return setError(op, REGISTER_NOT_ALLOWED);
        }
        if (val16 == 0 && !op.getError())
            return setError(op, OPERAND_NOT_ALLOWED);
        if (val16 >= 16)
            return setError(op, OVERFLOW_RANGE);
        insn.embed(val16 << 4);
        return OK;
    case M_CNT2:
        insn.embedPost(0x4000);
        if (op.mode == M_REG) {
            if (op.reg == REG_R0)
                return OK;
            return setError(op, REGISTER_NOT_ALLOWED);
        }
        if (val16 == 0 && !op.getError())
            return setError(op, OPERAND_NOT_ALLOWED);
        /* Fall-through */
    case M_BIT2:
        if (val16 >= 16)
            return setError(op, OVERFLOW_RANGE);
        insn.embedPost(val16 << 6);
        return OK;
    case M_RTWP:
        // 0,1,2,4 are acceptable.
        if (val16 != 3 && val16 <= 4 && op.isOK()) {
            insn.embed(val16 & 7);
            return OK;
        }
        return setError(op, OPERAND_NOT_ALLOWED);
    default:
        return OK;
    }
}

Error AsmTms9900::parseOperand(StrScanner &scan, Operand &op) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    if (p.expect('*')) {
        op.reg = RegTms9900::parseRegName(p);
        if (op.reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        op.mode = p.expect('+') ? M_INCR : M_IREG;
        scan = p;
        return OK;
    }
    if (p.expect('@')) {
        op.val16 = parseExpr16(p, op);
        if (parserError())
            return op.getError();
        if (p.skipSpaces().expect('(')) {
            const StrScanner regp = p.skipSpaces();
            op.reg = RegTms9900::parseRegName(p);
            if (op.reg == REG_UNDEF)
                return op.setError(UNKNOWN_OPERAND);
            if (op.reg == REG_R0)
                return op.setError(regp, REGISTER_NOT_ALLOWED);
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = M_INDX;
        } else {
            op.mode = M_SYBL;
        }
        scan = p;
        return OK;
    }
    op.reg = RegTms9900::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        op.mode = M_REG;
        scan = p;
        return OK;
    }

    op.val16 = parseExpr16(p, op);
    if (parserError())
        return op.getError();
    op.mode = M_IMM;
    scan = p;
    return OK;
}

Error AsmTms9900::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnTms9900 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    Operand srcOp, dstOp;
    if (parseOperand(scan, srcOp) && srcOp.hasError())
        return setError(srcOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, dstOp) && dstOp.hasError())
            return setError(dstOp);
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(srcOp);
    setErrorIf(dstOp);

    insn.setAddrMode(srcOp.mode, dstOp.mode);
    const auto error = TableTms9900::TABLE.searchName(insn);
    if (error)
        return setError(srcOp, error);

    const AddrMode src = insn.src();
    if (src != M_NONE && encodeOperand(insn, srcOp, src))
        return getError();
    const AddrMode dst = insn.dst();
    if (dst != M_NONE && encodeOperand(insn, dstOp, dst)) {
        insn.reset();
        return getError();
    }
    insn.emitInsn();
    return getError();
}

}  // namespace tms9900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
