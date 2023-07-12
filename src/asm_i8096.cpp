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

#include "operators.h"
#include "reg_i8096.h"
#include "table_i8096.h"
#include "text_common.h"

namespace libasm {
namespace i8096 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

constexpr Pseudo PSEUDOS[] PROGMEM = {
        Pseudo{TEXT_ALIGN, &Assembler::alignOrigin},
        Pseudo{TEXT_DCB, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
        Pseudo{TEXT_DCL, &Assembler::defineDataConstant, Assembler::DATA_LONG},
        Pseudo{TEXT_DCW, &Assembler::defineDataConstant, Assembler::DATA_WORD},
        Pseudo{TEXT_DSB, &Assembler::allocateSpaces, Assembler::DATA_BYTE},
        Pseudo{TEXT_DSL, &Assembler::allocateSpaces, Assembler::DATA_LONG},
        Pseudo{TEXT_DSW, &Assembler::allocateSpaces, Assembler::DATA_WORD},
        Pseudo{TEXT_ORG, &Assembler::defineOrigin},
};

}  // namespace

struct AsmI8096::Operand final : ErrorAt {
    AddrMode mode;
    uint8_t regno;
    Error regerr;
    uint16_t val16;
    Operand() : mode(M_NONE), regno(0), regerr(OK), val16(0) {}
};

const ValueParser::Plugins &AsmI8096::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const SimpleSymbolParser _symbol{SymbolParser::QUESTION};
        const OperatorParser &operators() const override {
            return IntelOperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmI8096::AsmI8096(const ValueParser::Plugins &plugins)
    : Assembler(plugins, ARRAY_RANGE(PSEUDOS)), Config(TABLE) {
    reset();
}

Error AsmI8096::parseIndirect(StrScanner &scan, Operand &op) const {
    Operand regop;
    op.regno = parseExpr16(scan, regop);
    if (regop.getError())
        op.setErrorIf(regop);
    if (scan.skipSpaces().expect(']'))
        return OK;
    return op.setError(scan, MISSING_CLOSING_BRACKET);
}

Error AsmI8096::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val16 = parseExpr16(p, op);
        if (op.hasError())
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
    if (op.hasError())
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

void AsmI8096::emitAop(AsmInsn &insn, AddrMode mode, const Operand &op) {
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
        if (op.isOK() && !overflowInt8(static_cast<int16_t>(op.val16))) {
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

void AsmI8096::emitRelative(AsmInsn &insn, AddrMode mode, const Operand &op) {
    if (mode == M_REL8) {
        // Jx: 2 bytes, DJNZ/JBx: 3 bytes
        const auto base = insn.address() + ((insn.opCode() & 0xF0) == 0xD0 ? 2 : 3);
        const auto target = op.getError() ? base : op.val16;
        const auto delta = branchDelta(base, target, op);
        if (overflowInt8(delta))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
    } else if (mode == M_REL11) {
        const auto base = insn.address() + 2;
        const auto target = op.getError() ? base : op.val16;
        const auto delta = branchDelta(base, target, op);
        if (overflowInt(delta, 11))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(static_cast<uint8_t>(delta >> 8) & 7);
        insn.emitOperand8(delta);
    } else {  // M_REL16
        const auto base = insn.address() + 3;
        const auto target = op.getError() ? base : op.val16;
        const auto delta = branchDelta(base, target, op);
        if (overflowInt16(delta))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand16(delta);
    }
}

void AsmI8096::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op) {
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
    AsmInsn insn(_insn);
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
    const auto error = TABLE.searchName(cpuType(), insn);
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
