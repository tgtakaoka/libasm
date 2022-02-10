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

#include "error_reporter.h"
#include "reg_i8096.h"
#include "table_i8096.h"

namespace libasm {
namespace i8096 {

Error AsmI8096::parseIndirect(const char *scan, Operand &op) {
    op.regno = parseExpr16(scan + 1);
    if (parserError())
        return getError();
    op.setError(getError());
    const char *p = skipSpaces(_scan);
    if (*p != ']')
        return setError(MISSING_CLOSING_BRACKET);
    _scan = p + 1;
    return OK;
}

Error AsmI8096::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    if (*p == '#') {
        op.val16 = parseExpr16(p + 1);
        if (parserError())
            return getError();
        op.setError(getError());
        op.mode = M_IMM16;
        return OK;
    }
    if (*p == '[') {
        if (parseIndirect(p, op))
            return getError();
        op.val16 = op.regno;
        op.mode = M_INDIR;
        return OK;
    }
    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    p = skipSpaces(_scan);
    if (*p == '[') {
        if (parseIndirect(p, op))
            return getError();
        op.mode = M_IDX16;
        return OK;
    }
    op.mode = M_ADDR;
    return OK;
}

Error AsmI8096::emitAop(InsnI8096 &insn, AddrMode mode, const Operand &op) {
    static constexpr auto ST = 0xC0;
    static constexpr auto STB = 0xC4;
    static constexpr auto POP = 0xCC;
    const bool waop = (mode == M_WAOP);
    switch (op.mode) {
    case M_IMM16:
        if (insn.opCode() == ST || insn.opCode() == STB || insn.opCode() == POP)
            return setError(OPERAND_NOT_ALLOWED);
        insn.embedAa(AA_IMM);
        if (waop) {
            insn.emitOperand16(op.val16);
        } else {
            insn.emitOperand8(op.val16);
        }
        return OK;
    case M_INDIR:
        insn.embedAa(AA_INDIR);
        insn.emitOperand8(op.regno);
        return OK;
    case M_IDX16:
        insn.embedAa(AA_IDX);
        if (!overflowRel8(static_cast<int16_t>(op.val16))) {
            insn.emitOperand8(op.regno);
            insn.emitOperand8(op.val16);
        } else {
            insn.emitOperand8(op.regno | 1);
            insn.emitOperand16(op.val16);
        }
        return OK;
    default:  // M_ADDR
        if (!overflowUint8(op.val16)) {
            insn.embedAa(AA_REG);
            insn.emitOperand8(op.val16);
        } else {
            insn.embedAa(AA_IDX);
            insn.emitOperand8(0 | 1);  // Zero register
            insn.emitOperand16(op.val16);
        }
        return OK;
    }
}

Error AsmI8096::emitRelative(InsnI8096 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t target = op.val16;
    Config::uintptr_t base;
    Config::ptrdiff_t delta;
    switch (mode) {
    case M_REL8:
        // Jx: 2 bytes, DJNZ/JBx: 3 bytes
        base = insn.address() + ((insn.opCode() & 0xF0) == 0xD0 ? 2 : 3);
        delta = target - base;
        if (overflowRel8(delta))
            return setError(OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return OK;
    case M_REL11:
        base = insn.address() + 2;
        delta = target - base;
        if (delta < -0x400 || delta >= 0x400)
            return setError(OPERAND_TOO_FAR);
        insn.embed(static_cast<uint8_t>(delta >> 8) & 7);
        insn.emitOperand8(delta);
        return OK;
    default:  // M_REL16
        base = insn.address() + 3;
        delta = target - base;
        insn.emitOperand16(delta);
        return OK;
    }
}

Error AsmI8096::emitOperand(InsnI8096 &insn, AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_LREG:
        if (op.val16 & 3)
            return setError(REGISTER_NOT_ALLOWED);
        // Fall-through
    case M_INDIR:
    case M_WREG:
        if (op.val16 & 1)
            return setError(REGISTER_NOT_ALLOWED);
        // Fall-through
    case M_COUNT:
    case M_BREG:
        if (op.val16 >= 0x100)
            return setError(ILLEGAL_REGISTER);
        insn.emitOperand8(op.val16);
        return OK;
    case M_BAOP:
    case M_WAOP:
        return emitAop(insn, mode, op);
    case M_REL8:
    case M_REL11:
    case M_REL16:
        return emitRelative(insn, mode, op);
    case M_BITNO:
        if (op.val16 >= 8)
            return setError(ILLEGAL_BIT_NUMBER);
        insn.embed(op.val16 & 7);
        return OK;
    default:
        return OK;
    }
}

Error AsmI8096::encode(Insn &_insn) {
    InsnI8096 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand dst, src1, src2;
    if (parseOperand(endName, dst))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, src1))
            return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, src2))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setError(dst.getError());
    setErrorIf(src1.getError());
    setErrorIf(src2.getError());

    insn.setAddrMode(dst.mode, src1.mode, src2.mode);
    if (TableI8096.searchName(insn))
        return setError(TableI8096.getError());

    const bool jbx_djnz = insn.src2() == M_REL8 || insn.src1() == M_REL8;
    if (!jbx_djnz && emitOperand(insn, insn.src2(), src2))
        return getError();
    if (!jbx_djnz && emitOperand(insn, insn.src1(), src1))
        return getError();
    if (emitOperand(insn, insn.dst(), dst))
        return getError();
    if (jbx_djnz && emitOperand(insn, insn.src1(), src1))
        return getError();
    if (jbx_djnz && emitOperand(insn, insn.src2(), src2))
        return getError();
    insn.emitInsn();
    return getError();
}

}  // namespace i8096
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
