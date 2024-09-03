/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "asm_tms32010.h"

#include "table_tms32010.h"
#include "text_common.h"

namespace libasm {
namespace tms32010 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dALIGN,  &Assembler::alignOrigin},
    {TEXT_dBYTE,   &Assembler::defineDataConstant, Assembler::DATA_BYTE_IN_WORD},
    {TEXT_dLONG,   &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_dORG,    &Assembler::defineOrigin},
    {TEXT_dSTRING, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_dWORD,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct Tms32010CommentParser final : CommentParser {
    bool commentLine(StrScanner &scan) const override { return *scan == '*'; }
    bool endOfLine(StrScanner &scan) const override {
        return SemicolonCommentParser::singleton().endOfLine(scan);
    }
};

struct Tms32010SymbolParser final : SimpleSymbolParser {
    Tms32010SymbolParser() : SimpleSymbolParser(PSTR_UNDER_DOLLAR) {}
    bool instructionLetter(char c) const override {
        return SimpleSymbolParser::instructionLetter(c) || c == '.';
    }
};

}  // namespace

const ValueParser::Plugins &AsmTms32010::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const CommentParser &comment() const override { return _comment; }
        const SymbolParser &symbol() const override { return _symbol; }
        const LetterParser &letter() const override { return _letter; }
        const Tms32010CommentParser _comment{};
        const Tms32010SymbolParser _symbol{};
        const struct : LetterParser {
            char stringDelimiter(StrScanner &scan) const override { return scan.expect('"'); }
        } _letter{};
    } PLUGINS{};
    return PLUGINS;
}

AsmTms32010::AsmTms32010(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

namespace {

AddrMode constantType(uint16_t val) {
    if (val == 0)
        return M_LS0;
    if (val == 1)
        return M_DPK;
    if (val == 4)
        return M_LS3;
    if (val < 8)
        return M_IM3;
    if (val < 16)
        return M_LS4;
    if (val < 0x100)
        return M_IM8;
    return M_IM13;
}

}  // namespace

void AsmTms32010::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    static constexpr Config::opcode_t SST = 0x7C00;
    switch (mode) {
    case M_MAM:
        switch (op.mode) {
        case M_ARP:
            insn.embed(0x88);
            break;
        case M_INC:
            insn.embed(0xA8);
            break;
        case M_DEC:
            insn.embed(0x98);
            break;
        default:
            if (op.val.getUnsigned() > dataMemoryLimit())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            if (insn.opCode() == SST && op.val.getUnsigned() < 0x80)
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(op.val.getUnsigned() & 0x7F);
            break;
        }
        break;
    case M_LS3:
    case M_LS4:
    case M_PA:
        insn.embed(op.val.getUnsigned() << 8);
        break;
    case M_NARP:
        if (op.mode == M_NONE)
            break;
        insn.setOpCode(insn.opCode() & ~8);
        insn.embed(op.val.getUnsigned());
        break;
    case M_AR:
        insn.embed(op.val.getUnsigned() << 8);
        break;
    case M_ARK:
    case M_DPK:
        insn.embed(op.val.getUnsigned());
        break;
    case M_IM8:
        insn.embed(op.val.getUnsigned());
        break;
    case M_IM13:
        if (op.val.overflow(0x0FFF, -0x1000))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(op.val.getUnsigned() & 0x1FFF);
        break;
    case M_PMA:
        if (op.val.overflow(0x0FFF))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned() & 0x0FFF);
        break;
    default:
        break;
    }
}

Error AsmTms32010::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('*')) {
        if (p.expect('+')) {
            op.mode = M_INC;
        } else if (p.expect('-')) {
            op.mode = M_DEC;
        } else {
            op.mode = M_ARP;
        }
        scan = p;
        return OK;
    }
    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        if (isAuxiliary(op.reg)) {
            op.mode = M_AR;
            op.val.setUnsigned(int8_t(op.reg) - int8_t(REG_AR0));
        } else {
            op.mode = M_PA;
            op.val.setUnsigned(int8_t(op.reg) - int8_t(REG_PA0));
        }
        scan = p;
        return OK;
    }

    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = constantType(op.val.getUnsigned());
    scan = p;
    return OK;
}

Error AsmTms32010::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.op1) && insn.op1.hasError())
        return _insn.setError(insn.op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.op2) && insn.op2.hasError())
            return _insn.setError(insn.op2);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.op3) && insn.op3.hasError())
            return _insn.setError(insn.op3);
        scan.skipSpaces();
    }

    if (_insn.setErrorIf(insn.op1, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    encodeOperand(insn, insn.op1, insn.mode1());
    encodeOperand(insn, insn.op2, insn.mode2());
    encodeOperand(insn, insn.op3, insn.mode3());
    insn.emitInsn();
    return _insn.setErrorIf(insn);
}

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
