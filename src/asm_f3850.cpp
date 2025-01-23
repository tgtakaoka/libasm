/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "asm_f3850.h"

#include "reg_f3850.h"
#include "table_f3850.h"
#include "text_common.h"

namespace libasm {
namespace f3850 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char TEXT_RS[] PROGMEM = "rs";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DA, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_DC, &Assembler::defineDataConstant, Assembler::DATA_BYTE_OR_WORD},
    {TEXT_RS, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmF3850::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return FairchildNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return FairchildSymbolParser::singleton(); }
        const CommentParser &comment() const override { return StarCommentParser::singleton(); }
        const LetterParser &letter() const override { return FairchildLetterParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmF3850::AsmF3850(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Error AsmF3850::parseOperand(StrScanner &scan, Operand &op) const {
    op.setAt(scan.skipSpaces());
    auto p = scan;

    auto reg = parseRegName(p, parser());
    if (reg != REG_UNDEF) {
        if (int8_t(reg) < int8_t(REG_alias)) {
            op.mode = AddrMode(uint8_t(reg));
        } else if (reg == REG_J) {
            op.mode = M_J;
        } else {
            op.mode = M_REG;
        }
        op.val.setUnsigned(int8_t(reg) - int8_t(REG_alias));
        scan = p;
        return OK;
    }

    op.val = parseInteger(p.skipSpaces(), op);
    if (op.hasError())
        return op.getError();
    const auto v = op.val.getSigned();
    if (v == 1) {
        op.mode = M_C1;
    } else if (v == 4) {
        op.mode = M_C4;
    } else if (op.val.overflow(7)) {
        op.mode = M_IM3;
    } else if (op.val.overflow(15)) {
        op.mode = M_IM4;
    } else if (!op.val.overflowUint8()) {
        op.mode = M_IM8;
    } else if (!op.val.overflowUint16()) {
        op.mode = M_ADDR;
    } else {
        op.setError(OVERFLOW_RANGE);
    }
    scan = p;
    return OK;
}

void AsmF3850::emitRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + 1;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
}

void AsmF3850::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    const auto v = op.val.getUnsigned();
    switch (mode) {
    case M_REG:
        if (op.val.overflow(15))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        if (v == 15)
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        insn.embed(v & 0xF);
        break;
    case M_C1:
        if (v == 1) {
            ;  // nop
        } else if (v == 4) {
            insn.setOpCode(insn.opCode() + 2);
        } else {
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
        break;
    case M_IM3:
        if (op.val.overflow(7))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(v & 7);
        break;
    case M_IOS:
        if (v == 2 || v == 3)
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        /* Fall-through */
    case M_IM4:
        if (op.val.overflow(15))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(v & 0xF);
        break;
    case M_IOA:
        if (op.isOK() && !op.val.overflow(3, 0))
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        /* Fall-through */
    case M_IM8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(v);
        break;
    case M_ADDR:
        insn.emitOperand16(v);
        break;
    case M_REL:
        emitRelative(insn, op);
        break;
    default:
        break;
    }
}

Error AsmF3850::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (TABLE.hasOperand(cpuType(), insn)) {
        if (parseOperand(scan, insn.op1) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op2) && insn.op2.hasError())
                return _insn.setError(insn.op2);
        }
    }
    scan.skipSpaces();

    if (_insn.setErrorIf(insn.op1, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    encodeOperand(insn, insn.op1, insn.mode1());
    encodeOperand(insn, insn.op2, insn.mode2());
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace f3850
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
