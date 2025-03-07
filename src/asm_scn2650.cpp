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

#include "asm_scn2650.h"
#include "table_scn2650.h"
#include "text_common.h"

namespace libasm {
namespace scn2650 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char TEXT_ACON[] PROGMEM = "acon";
constexpr char TEXT_ALIT[] PROGMEM = "alit";
constexpr char TEXT_RES[]  PROGMEM = "res";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_ACON, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_ALIT, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DATA, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_RES,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmScn2650::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return SigneticsNumberParser::singleton(); }
        const CommentParser &comment() const override { return StarCommentParser::singleton(); }
        const LetterParser &letter() const override { return SigneticsLetterParser::singleton(); }
        const OperatorParser &operators() const override {
            return SigneticsOperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmScn2650::AsmScn2650(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Error AsmScn2650::parseOperand(StrScanner &scan, Operand &op) const {
    // Do not skip preceding spaces.
    op.setAt(scan);
    if (endOfLine(scan))
        return OK;

    auto p = scan;
    op.reg = parseRegName(p, parser());
    if (op.reg != REG_UNDEF) {
        op.mode = op.reg == REG_R0 ? M_REG0 : M_R123;
        scan = p;
        return OK;
    }
    op.cc = parseCcName(p, parser());
    if (op.cc != CC_UNDEF) {
        op.mode = op.cc == CC_UN ? M_CCVN : M_C012;
        scan = p;
        return OK;
    }

    op.indir = p.expect('*');
    op.val = parseInteger(p.skipSpaces(), op);
    if (op.hasError())
        return op.getError();
    if (p.expect(',')) {
        op.reg = parseRegName(p.skipSpaces(), parser());
        if (op.reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        if (p.skipSpaces().expect(',')) {
            op.sign = p.skipSpaces().expect([](char c) { return c == '+' || c == '-'; });
            if (op.sign) {
                op.mode = M_IX13;
                scan = p;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        op.mode = op.reg == REG_R3 ? M_IX15 : M_IX13;
        scan = p;
        return OK;
    }
    if (op.indir) {
        op.mode = M_AB15;
    } else {
        op.mode = M_IMM8;
    }
    scan = p;
    return OK;
}

void AsmScn2650::emitAbsolute(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto target = op.getError() ? insn.address() : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target));
    auto opr = target;
    if (op.indir)
        opr |= 0x8000;
    if (mode == M_IX15) {
        if (op.reg != REG_R3 && op.reg != REG_UNDEF)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.embed(encodeRegName(REG_R3));
    }
    insn.emitOperand16(opr);
}

void AsmScn2650::emitIndexed(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto target = op.getError() ? insn.address() : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target, insn.address(), 13));
    auto opr = offset(target);
    if (op.indir)
        opr |= 0x8000;
    if (mode == M_IX13) {
        insn.embed(encodeRegName(op.reg));
        switch (op.sign) {
        case '+':
            opr |= 0x2000;
            break;
        case '-':
            opr |= 0x4000;
            break;
        default:
            opr |= 0x6000;
            break;
        }
    }
    insn.emitOperand16(opr);
}

void AsmScn2650::emitZeroPage(AsmInsn &insn, const Operand &op) const {
    const auto target = op.val.getUnsigned();
    if (page(target) != 0)
        insn.setErrorIf(op, OVERFLOW_RANGE);
    // Sign extends 13-bit number
    const auto offset = signExtend(target, 13);
    if (overflowDelta(offset, 7))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    uint8_t opr = target & 0x7F;
    if (op.indir)
        opr |= 0x80;
    insn.emitOperand8(opr);
}

void AsmScn2650::emitRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = inpage(insn.address(), 2);
    const auto target = op.getError() ? base : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target, insn.address(), 13));
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowDelta(delta, 7))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    uint8_t opr = delta & 0x7F;
    if (op.indir)
        opr |= 0x80;
    insn.emitOperand8(opr);
}

void AsmScn2650::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_REGN:
    case M_REG0:
    case M_R123:
        insn.embed(encodeRegName(op.reg));
        break;
    case M_CCVN:
    case M_C012:
        insn.embed(encodeCcName(op.cc));
        break;
    case M_IMM8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_REL7:
        emitRelative(insn, op);
        break;
    case M_ABS7:
        emitZeroPage(insn, op);
        break;
    case M_AB13:
    case M_IX13:
        emitIndexed(insn, op, mode);
        break;
    case M_AB15:
    case M_IX15:
        emitAbsolute(insn, op, mode);
        break;
    default:
        break;
    }
}

Error AsmScn2650::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    const auto comma = scan.expect(',');
    if (hasOperand(cpuType(), insn)) {
        if (!comma)
            scan.skipSpaces();
        if (parseOperand(scan, insn.op1) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        if (insn.mode2() != M_NONE) {
            if (comma || scan.skipSpaces().expect(',')) {
                if (parseOperand(scan.skipSpaces(), insn.op2) && insn.op2.hasError())
                    return _insn.setError(insn.op2);
            }
        }
    }
    scan.skipSpaces();

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.op1, insn);

    const auto mode1 = insn.mode1();
    if (comma && (mode1 == M_REGN || mode1 == M_REG0 || mode1 == M_R123))
        outRegName(insn.nameBuffer().letter(','), insn.op1.reg);
    encodeOperand(insn, insn.op1, insn.mode1());
    encodeOperand(insn, insn.op2, insn.mode2());
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace scn2650
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
