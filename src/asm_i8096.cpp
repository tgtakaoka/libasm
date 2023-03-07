/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "asm_i8096.h"

namespace libasm {
namespace i8096 {

Error AsmI8096::parseIndirect(StrScanner &scan, Operand &op) const {
    Operand regop;
    op.regno = parseExpr16(scan, regop);
    if (parserError())
        return op.setError(regop);
    if (regop.getError())
        op.setErrorIf(regop);
    if (scan.skipSpaces().expect(']'))
        return OK;
    return op.setError(scan, MISSING_CLOSING_BRACKET);
}

Error AsmI8096::parseOperand(StrScanner &scan, Operand &op) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val16 = parseExpr16(p, op);
        if (parserError())
            return op.getError();
        op.mode = M_IMM16;
        scan = p;
        return OK;
    }
    if (p.expect('[')) {
        if (parseIndirect(p, op))
            return op.getError();
        op.val16 = op.regno;
        op.mode = M_INDIR;
        scan = p;
        return OK;
    }
    op.val16 = parseExpr16(p, op);
    if (parserError())
        return op.getError();
    if (p.skipSpaces().expect('[')) {
        if (parseIndirect(p, op))
            return op.getError();
        op.mode = M_IDX16;
    } else {
        op.mode = M_ADDR;
    }
    scan = p;
    return OK;
}

void AsmI8096::emitAop(InsnI8096 &insn, AddrMode mode, const Operand &op) {
    const auto waop = (mode == M_WAOP);
    switch (op.mode) {
    case M_IMM16:
        insn.embedAa(AA_IMM);
        if (waop) {
            insn.emitOperand16(op.val16);
        } else {
            insn.emitOperand8(op.val16);
        }
        return;
    case M_INDIR:
    indir:
        insn.embedAa(AA_INDIR);
        insn.emitOperand8(op.regno);
        return;
    case M_IDX16:
        if (op.isOK() && op.val16 == 0)
            goto indir;
        insn.embedAa(AA_IDX);
        if (op.isOK() && !overflowRel8(static_cast<int16_t>(op.val16))) {
            insn.emitOperand8(op.regno);
            insn.emitOperand8(op.val16);
        } else {
            insn.emitOperand8(op.regno | 1);
            insn.emitOperand16(op.val16);
        }
        return;
    default:  // M_ADDR
        if (op.isOK() && !overflowUint8(op.val16)) {
            insn.embedAa(AA_REG);
            insn.emitOperand8(op.val16);
        } else {
            insn.embedAa(AA_IDX);
            insn.emitOperand8(0 | 1);  // Zero register
            insn.emitOperand16(op.val16);
        }
        return;
    }
}

void AsmI8096::emitRelative(InsnI8096 &insn, AddrMode mode, const Operand &op) {
    Config::uintptr_t target;
    Config::uintptr_t base;
    Config::ptrdiff_t delta;
    switch (mode) {
    case M_REL8:
        // Jx: 2 bytes, DJNZ/JBx: 3 bytes
        base = insn.address() + ((insn.opCode() & 0xF0) == 0xD0 ? 2 : 3);
        target = op.getError() ? base : op.val16;
        delta = target - base;
        if (overflowRel8(delta))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return;
    case M_REL11:
        base = insn.address() + 2;
        target = op.getError() ? base : op.val16;
        delta = target - base;
        if (delta < -0x400 || delta >= 0x400)
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(static_cast<uint8_t>(delta >> 8) & 7);
        insn.emitOperand8(delta);
        return;
    default:  // M_REL16
        base = insn.address() + 3;
        target = op.getError() ? base : op.val16;
        delta = target - base;
        insn.emitOperand16(delta);
        return;
    }
}

void AsmI8096::emitOperand(InsnI8096 &insn, AddrMode mode, const Operand &op) {
    auto val16 = op.val16;
    switch (mode) {
    case M_LREG:
        if (val16 & 3) {
            val16 &= ~3;
            setErrorIf(op, REGISTER_NOT_ALLOWED);
        }
        // Fall-through
    case M_INDIR:
    case M_WREG:
        if (val16 & 1) {
            val16 &= ~1;
            setErrorIf(op, REGISTER_NOT_ALLOWED);
        }
        // Fall-through
    case M_COUNT:
    case M_BREG:
        if (val16 >= 0x100)
            setErrorIf(op, ILLEGAL_REGISTER);
        insn.emitOperand8(val16);
        return;
    case M_BAOP:
    case M_WAOP:
        emitAop(insn, mode, op);
        return;
    case M_REL8:
    case M_REL11:
    case M_REL16:
        emitRelative(insn, mode, op);
        return;
    case M_BITNO:
        if (op.val16 >= 8)
            setErrorIf(op, ILLEGAL_BIT_NUMBER);
        insn.embed(op.val16 & 7);
        return;
    default:
        return;
    }
}

Error AsmI8096::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnI8096 insn(_insn);
    Operand dst, src1, src2;
    if (parseOperand(scan, dst) && dst.hasError())
        return setError(dst);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, src1) && src1.hasError())
            return setError(src1);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, src2) && src2.hasError())
            return setError(src2);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(dst);
    setErrorIf(src1);
    setErrorIf(src2);

    insn.setAddrMode(dst.mode, src1.mode, src2.mode);
    const auto error = TableI8096::TABLE.searchName(insn);
    if (error)
        return setError(dst, error);

    const auto jbx_djnz = insn.src2() == M_REL8 || insn.src1() == M_REL8;
    if (!jbx_djnz) {
        emitOperand(insn, insn.src2(), src2);
        emitOperand(insn, insn.src1(), src1);
    }
    emitOperand(insn, insn.dst(), dst);
    if (jbx_djnz) {
        emitOperand(insn, insn.src1(), src1);
        emitOperand(insn, insn.src2(), src2);
    }
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace i8096
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
