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

#include "asm_tms320.h"
#include "table_tms320.h"
#include "text_common.h"

namespace libasm {
namespace tms320 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dALIGN,  &Assembler::alignOrigin},
    {TEXT_dBYTE,   &Assembler::defineDataConstant, Assembler::DATA_BYTE_IN_WORD},
    {TEXT_dEVEN,   &Assembler::alignOrigin,        Assembler::ALIGN_EVEN},
    {TEXT_dLONG,   &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_dORG,    &Assembler::defineOrigin},
    {TEXT_dSTRING, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_dWORD,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmTms320::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const CommentParser &comment() const override { return Tms320CommentParser::singleton(); }
        const SymbolParser &symbol() const override { return Tms320SymbolParser::singleton(); }
        const LetterParser &letter() const override { return Tms320LetterParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmTms320::AsmTms320(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmTms320::encodeIndirect(AsmInsn &insn, const Operand &op) const {
    static constexpr uint8_t MAR[] PROGMEM = {
            0x80,  // M_ARP: *
            0xA0,  // M_INC: *+
            0x90,  // M_DEC: *-
            0xE0,  // M_INC0: *0+
            0xD0,  // M_DEC0: *0-
            0xF0,  // M_IBR0: *BR0+
            0xC0,  // M_DBR0: *BR0-
    };
    if (op.mode >= M_ARP)
        insn.embed(pgm_read_byte(&MAR[op.mode - M_ARP]));
}

void AsmTms320::encodeDirect(AsmInsn &insn, const Operand &op) const {
    const auto dma = op.val.getUnsigned();
    if (op.val.isNegative() || !validDmAddr(insn.opCode(), dma))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.embed(dma & 0x7F);
}

void AsmTms320::encodeNextAR(AsmInsn &insn, const Operand &op) const {
    if ((insn.opCode() & 0x80) == 0)
        return;
    if (op.mode == M_NONE) {
        if (is3201x())
            insn.embed(8);
        return;
    }
    auto val = op.val.getUnsigned();
    if (!isAR(decodeAR(val))) {
        val &= maxAR();
        insn.setErrorIf(op, UNKNOWN_REGISTER);
    }
    if (is3202x())
        insn.embed(8);
    insn.embed(val);
}

void AsmTms320::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    auto val = op.val.getUnsigned();
    auto max = UINT16_MAX;
    switch (mode) {
    case M_MAM:
        if (op.mode == M_CNST) {
            encodeDirect(insn, op);
        } else {
            encodeIndirect(insn, op);
        }
        break;
    case M_IND:
    case M_MAR:
        encodeIndirect(insn, op);
        break;
    case M_LS0:
        if (val)
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
        break;
    case M_LS3:
        if (!is320C2x() && !(val == 0 || val == 1 || val == 4))
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
        // Fall-through
    case M_PA:
        if (!isPA(decodePA(val))) {
            val &= maxPA();
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        // Fall-through
    case M_LS4:
        if (op.val.overflow(15)) {
            val &= 15;
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
    embed_hi8:
        insn.embed(val << 8);
        break;
    case M_NARP:
        encodeNextAR(insn, op);
        break;
    case M_AR:
    case M_ARK:
        if (!isAR(decodeAR(val))) {
            val &= maxAR();
            insn.setErrorIf(op, UNKNOWN_REGISTER);
        }
        if (mode == M_AR)
            goto embed_hi8;
        goto embed_const;
    case M_IM1:
        max = 1;
    check_const:
        if (op.val.overflow(max)) {
            val &= max;
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
    embed_const:
        insn.embed(val);
        break;
    case M_IM2:
        max = 3;
        goto check_const;
    case M_BIT:
        max = 15;
        goto embed_hi8;
    case M_IM8:
        max = UINT8_MAX;
        goto check_const;
    case M_IM9:
        max = 0x1FF;
        goto check_const;
    case M_IM13:
        if (op.val.overflow(0x0FFF, -0x1000))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(val & 0x1FFF);
        break;
    case M_PM12:
        if (op.val.overflow(0x0FFF)) {
            val &= 0x0FFF;
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        // Fall-through
    case M_PM16:
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        // Fall-through
    case M_IM16:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(val);
        break;
    default:
        break;
    }
}

Error AsmTms320::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('*')) {
        auto s = p;
        const auto br = p.iexpect('B') && p.iexpect('R');
        if (!br)
            p = s;
        const auto ar0 = p.expect('0');
        if (br && !ar0)
            return op.setErrorIf(UNKNOWN_OPERAND);
        if (p.expect('+')) {
            op.mode = ar0 ? (br ? M_IBR0 : M_INC0) : M_INC;
        } else if (p.expect('-')) {
            op.mode = ar0 ? (br ? M_DBR0 : M_DEC0) : M_DEC;
        } else {
            if (br || ar0)
                return op.setErrorIf(UNKNOWN_OPERAND);
            op.mode = M_ARP;
        }
        scan = p;
        return OK;
    }
    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        op.mode = isAR(op.reg) ? M_AR : M_PA;
        op.val.setUnsigned(encodeRegName(op.reg));
        scan = p;
        return OK;
    }

    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_CNST;
    scan = p;
    return OK;
}

Error AsmTms320::encodeImpl(StrScanner &scan, Insn &_insn) const {
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

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.op1, insn);

    encodeOperand(insn, insn.op1, insn.mode1());
    encodeOperand(insn, insn.op2, insn.mode2());
    encodeOperand(insn, insn.op3, insn.mode3());
    insn.emitInsn();
    return _insn.setErrorIf(insn);
}

}  // namespace tms320
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
