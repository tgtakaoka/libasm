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

#include "asm_mc6800.h"

#include "reg_mc6800.h"
#include "table_mc6800.h"
#include "text_common.h"

namespace libasm {
namespace mc6800 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_FCB, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING},
    {TEXT_FCC, &Assembler::defineString},
    {TEXT_FDB, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING},
    {TEXT_RMB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmMc6800::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return MotorolaNumberParser::singleton(); }
        const CommentParser &comment() const override { return AsteriskCommentParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const LetterParser &letter() const override { return MotorolaLetterParser::singleton(); }
        const LocationParser &location() const override {
            return AsteriskLocationParser::singleton();
        }
        const OperatorParser &operators() const override {
            return Mc68xxOperatorParser::singleton();
        }
        const PrefixSymbolParser _symbol{PSTR_DOT, PSTR_UNDER_DOT_DOLLER};
    } PLUGINS{};
    return PLUGINS;
}

AsmMc6800::AsmMc6800(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Error AsmMc6800::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p) || *p == ',') {
        op.mode = M_NONE;
        scan = p;
        return OK;
    }

    if (p.expect('#')) {
        op.val16 = parseExpr16(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IM16;
        scan = p;
        return OK;
    }

    op.size = 0;
    if (p.expect('<')) {
        op.size = 8;
    } else if (p.expect('>')) {
        op.size = 16;
    }
    op.val16 = parseExpr16(p, op);
    if (op.hasError())
        return op.getError();

    auto a = p;
    if (a.skipSpaces().expect(',')) {
        const auto reg = parseRegName(a.skipSpaces());
        if (reg != REG_UNDEF) {
            op.mode = (reg == REG_X) ? M_IDX : M_IDY;
            scan = a;
            return OK;
        }
    }
    if (op.size == 0) {
        if (op.val16 < 8)
            op.mode = M_BIT;
        else if (op.val16 < 0x100)
            op.mode = M_DIR;
        else
            op.mode = M_EXT;
    } else {
        op.mode = (op.size == 8) ? M_DIR : M_EXT;
    }
    scan = p;
    return OK;
}

void AsmMc6800::emitRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + (insn.length() == 0 ? 2 : insn.length() + 1);
    const auto target = op.getError() ? base : op.val16;
    const auto delta = branchDelta(base, target, insn, op);
    if (overflowInt8(delta))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
}

void AsmMc6800::emitImmediate(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    if (mode == M_GN16 || mode == M_IM16) {
        insn.emitOperand16(op.val16);
    } else {
        if (overflowUint8(op.val16))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
    }
}

void AsmMc6800::emitBitNumber(AsmInsn &insn, const Operand &op) const {
    const uint8_t imm = 1U << (op.val16 & 7);
    const auto aim = (insn.opCode() & 0xF) == 1;
    insn.emitOperand8(aim ? ~imm : imm);
}

void AsmMc6800::emitOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_GN8:
    case M_GN16:
        if (op.mode == M_DIR || op.mode == M_BIT) {
            insn.embed(0x10);
            insn.emitOperand8(op.val16);
        } else if (op.mode == M_IDX || op.mode == M_IDY) {
            if (op.val16 >= 256)
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(0x20);
            insn.emitOperand8(op.val16);
        } else if (op.mode == M_EXT) {
            insn.embed(0x30);
            insn.emitOperand16(op.val16);
        } else {
            emitImmediate(insn, mode, op);
        }
        break;
    case M_GMEM:
        if (op.mode == M_IDX) {
            if (op.val16 >= 256)
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand8(op.val16);
        } else {
            insn.embed(0x70);
            insn.emitOperand16(op.val16);
        }
        break;
    case M_DIR:
    case M_IDX:
    case M_IDY:
        if (op.val16 >= 256)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        break;
    case M_EXT:
        insn.emitOperand16(op.val16);
        break;
    case M_REL:
        emitRelative(insn, op);
        break;
    case M_IM8:
    case M_IM16:
    case M_BMM:
        emitImmediate(insn, mode, op);
        break;
    case M_BIT:
        emitBitNumber(insn, op);
        break;
    default:
        break;
    }
}

Error AsmMc6800::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.op1) && insn.op1.hasError())
        return setError(insn.op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.op2) && insn.op2.hasError())
            return setError(insn.op2);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.op3) && insn.op3.hasError())
            return setError(insn.op3);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);

    const auto error = TABLE.searchName(cpuType(), insn);
    if (error)
        return setError(insn.op1, error);

    emitOperand(insn, insn.mode1(), insn.op1);
    emitOperand(insn, insn.mode2(), insn.op2);
    emitOperand(insn, insn.mode3(), insn.op3);
    insn.emitInsn();
    return setError(insn);
}

}  // namespace mc6800
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
