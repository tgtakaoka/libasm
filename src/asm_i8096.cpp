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
#include "reg_i8096.h"
#include "table_i8096.h"
#include "text_common.h"

namespace libasm {
namespace i8096 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DCB, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DCL, &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_DCW, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_DSB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DSL, &Assembler::allocateSpaces,     Assembler::DATA_LONG},
    {TEXT_DSW, &Assembler::allocateSpaces,     Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmI8096::defaultPlugins() {
    return ValueParser::Plugins::intel();
}

AsmI8096::AsmI8096(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Error AsmI8096::parseIndirect(
        StrScanner &scan, Operand &op, const StrScanner &at, bool autoInc) const {
    auto p = scan;
    if (!p.expect('['))
        return NOT_AN_EXPECTED;
    ErrorAt error;
    op.regno = parseInteger(p, error);
    op.setErrorIf(error);
    if (!p.skipSpaces().expect(']'))
        return op.setErrorIf(p, MISSING_CLOSING_BRACKET);
    if (p.skipSpaces().expect('+')) {
        if (!autoInc)
            return op.setErrorIf(at, OPERAND_NOT_ALLOWED);
        op.mode = M_AINC;
    } else {
        op.mode = M_INDIR;
    }
    scan = p;
    return OK;
}

Error AsmI8096::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMM16;
        scan = p;
        return OK;
    }
    if (parseIndirect(p, op, p, true) != NOT_AN_EXPECTED) {
        if (op.hasError())
            return op.getError();
        scan = p;
        op.val = op.regno;
        return OK;
    }

    auto at = p;
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    if (parseIndirect(p, op, at, false) != NOT_AN_EXPECTED) {
        if (op.hasError())
            return op.getError();
        op.mode = M_IDX16;
    } else {
        op.mode = M_ADDR;
    }
    scan = p;
    return OK;
}

void AsmI8096::emitAop(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto waop = (mode == M_WAOP);
    const auto val16 = op.val.getUnsigned();
    const auto regno = op.regno.getUnsigned();
    switch (op.mode) {
    case M_IMM16:
        insn.embedAa(AA_IMM);
        if (waop) {
            insn.emitOperand16(val16);
        } else {
            insn.emitOperand8(val16);
        }
        return;
    case M_INDIR:
    indir:
        insn.embedAa(AA_INDIR);
        if (!isWreg(regno))
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        insn.emitOperand8(regno);
        return;
        return;
    case M_AINC:
        insn.embedAa(AA_INDIR);
        if (!isWreg(regno))
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        insn.emitOperand8(regno | 1);
        return;
    case M_IDX16:
        if (op.val.isZero())
            goto indir;
        if (op.regno.isZero())
            goto absolute;
        insn.embedAa(AA_IDX);
        if (!isWreg(regno))
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        if (op.isOK() && !op.val.overflowInt8()) {
            insn.emitOperand8(regno);
            insn.emitOperand8(val16);
        } else {
            insn.emitOperand8(regno | 1);
            insn.emitOperand16(val16);
        }
        return;
    default:  // M_ADDR
    absolute:
        if (waop && !isWreg(val16))
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        if (op.isOK() && !op.val.overflowUint8()) {
            insn.embedAa(AA_REG);
            insn.emitOperand8(val16);
        } else {
            insn.embedAa(AA_IDX);
            insn.emitOperand8(0 | 1);  // Zero register
            insn.emitOperand16(val16);
        }
        return;
    }
}

namespace {

constexpr Config::opcode_t SJMP = 0x20;
constexpr Config::opcode_t LJMP = 0xE7;
constexpr Config::opcode_t SCALL = 0x28;
constexpr Config::opcode_t LCALL = 0xEF;

void shortBranch(AsmInsn &insn) {
    const auto opc = insn.opCode();
    if (opc == LJMP)
        insn.setOpCode(SJMP);
    else if (opc == LCALL)
        insn.setOpCode(SCALL);
}

void longBranch(AsmInsn &insn, const Operand &op) {
    const auto opc = insn.opCode();
    if (opc == SJMP)
        insn.setOpCode(LJMP);
    else if (opc == SCALL)
        insn.setOpCode(LCALL);
    insn.setError(op);
}

}  // namespace

void AsmI8096::emitRelative(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    if (mode == M_REL8) {
        // Jx: 2 bytes, DJNZ/JBx: 3 bytes
        const auto opc = insn.opCode();
        const auto base = insn.address() + ((opc & 0xF0) == 0xD0 ? 2 : 3);
        const auto target = op.getError() ? base : op.val.getUnsigned();
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return;
    }
    if (mode == M_REL11 && !_smartBranch) {
        const auto base = insn.address() + 2;
        const auto target = op.getError() ? base : op.val.getUnsigned();
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 11))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed(static_cast<uint8_t>(delta >> 8) & 7);
        insn.emitOperand8(delta);
        return;
    }
    if (mode == M_REL16 && !_smartBranch) {
    long_branch:
        const auto base = insn.address() + 3;
        const auto target = op.getError() ? base : op.val.getUnsigned();
        const auto delta = branchDelta(base, target, insn, op);
        insn.emitOperand16(delta);
        return;
    }
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (op.getError() || overflowDelta(delta, 11)) {
        longBranch(insn, op);
        goto long_branch;
    }
    shortBranch(insn);
    insn.embed(static_cast<uint8_t>(delta >> 8) & 7);
    insn.emitOperand8(delta);
}

void AsmI8096::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    insn.setErrorIf(op);
    auto val16 = op.val.getUnsigned();
    switch (mode) {
    case M_LREG:
        if (!isLreg(val16))
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        // Fall-through
    case M_INDIR:
    case M_WREG:
        if (!isWreg(val16))
            insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        // Fall-through
    case M_COUNT:
    case M_BREG:
        if (op.val.overflow(UINT8_MAX))
            insn.setErrorIf(op, ILLEGAL_REGISTER);
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
        if (op.val.overflow(7))
            insn.setErrorIf(op, ILLEGAL_BIT_NUMBER);
        insn.embed(op.val.getUnsigned() & 7);
        return;
    default:
        return;
    }
}

Error AsmI8096::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return _insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.src1Op) && insn.src1Op.hasError())
            return _insn.setError(insn.src1Op);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.src2Op) && insn.src2Op.hasError())
            return _insn.setError(insn.src2Op);
        scan.skipSpaces();
    }

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.dstOp, insn);

    const auto jbx_djnz = insn.src2() == M_REL8 || insn.src1() == M_REL8;
    if (!jbx_djnz) {
        insn.setErrorIf(insn.dstOp);
        insn.setErrorIf(insn.src1Op);
        emitOperand(insn, insn.src2(), insn.src2Op);
        emitOperand(insn, insn.src1(), insn.src1Op);
    }
    emitOperand(insn, insn.dst(), insn.dstOp);
    if (jbx_djnz) {
        emitOperand(insn, insn.src1(), insn.src1Op);
        emitOperand(insn, insn.src2(), insn.src2Op);
    }
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace i8096
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
