/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "asm_am29000.h"

#include "reg_am29000.h"
#include "table_am29000.h"
#include "letter_parser.h"
#include "symbol_parser.h"
#include "text_am29000.h"
#include "text_common.h"

namespace libasm {
namespace am29000 {

using namespace pseudo;
using namespace reg;
using namespace text::am29000;
using namespace text::common;

namespace {

// clang-format off
constexpr char TEXT_dHWORD[] PROGMEM = ".hword";
constexpr char TEXT_dSPACE[] PROGMEM = ".space";
constexpr char TEXT_FPU[]     PROGMEM = "fpu";
constexpr char TEXT_INTMULT[] PROGMEM = "intmult";

// The GNU assembler for this target defines a word as 32 bits and a half-word
// as 16, so the data directives follow that rather than the 16-bit "word" of
// the older families.
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_dASCII, &Assembler::defineDataConstant, Assembler::DATA_BYTE },
    { TEXT_dBYTE,  &Assembler::defineDataConstant, Assembler::DATA_BYTE },
    { TEXT_dHWORD, &Assembler::defineDataConstant, Assembler::DATA_WORD },
    { TEXT_dSPACE, &Assembler::allocateSpaces,     Assembler::DATA_BYTE },
    { TEXT_dWORD,  &Assembler::defineDataConstant, Assembler::DATA_LONG },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};
// clang-format on

}  // namespace

const AsmAm29000::PseudoAm29000 AsmAm29000::PSEUDO_AM29000_TABLE[] PROGMEM = {
        {TEXT_FPU, &AsmAm29000::processFpu},
        {TEXT_INTMULT, &AsmAm29000::processIntMul},
};
PROGMEM constexpr AsmAm29000::PseudosAm29000 AsmAm29000::PSEUDOS_AM29000{
        ARRAY_RANGE(PSEUDO_AM29000_TABLE)};

const ValueParser::Plugins &AsmAm29000::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const SymbolParser &symbol() const override { return Am29000SymbolParser::singleton(); }
        const LetterParser &letter() const override { return CStyleLetterParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmAm29000::AsmAm29000(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_fpu),
      Config(TABLE),
      _opt_fpu(this, &AsmAm29000::setFpuOption, OPT_BOOL_FPU, OPT_DESC_FPU, &_opt_intMul),
      _opt_intMul(this, &AsmAm29000::setIntMulOption, OPT_BOOL_INTMULT, OPT_DESC_INTMULT) {
    reset();
}

void AsmAm29000::reset() {
    Assembler::reset();
    // Return the arithmetic options to what the selected CPU implements.
    setCpuType(cpuType());
}

Error AsmAm29000::setFpuOption(bool enable) {
    return setFpu(enable);
}

Error AsmAm29000::setIntMulOption(bool enable) {
    return setIntMul(enable);
}

Error AsmAm29000::processFpu(StrScanner &scan, Insn &insn, uint16_t) {
    auto p = scan.skipSpaces();
    const auto at = p;
    bool on;
    if (p.expectTrue()) {
        on = true;
    } else if (p.expectFalse()) {
        on = false;
    } else {
        return insn.setError(at, UNKNOWN_OPERAND);
    }
    scan = p;
    return insn.setErrorIf(at, setFpu(on));
}


Error AsmAm29000::processIntMul(StrScanner &scan, Insn &insn, uint16_t) {
    auto p = scan.skipSpaces();
    const auto at = p;
    bool on;
    if (p.expectTrue()) {
        on = true;
    } else if (p.expectFalse()) {
        on = false;
    } else {
        return insn.setError(at, UNKNOWN_OPERAND);
    }
    scan = p;
    return insn.setErrorIf(at, setIntMul(on));
}

Error AsmAm29000::processPseudo(StrScanner &scan, Insn &insn) {
    const auto *p = PSEUDOS_AM29000.search(insn.name());
    return p ? p->invoke(this, scan, insn) : Assembler::processPseudo(scan, insn);
}

Error AsmAm29000::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    uint8_t regno;
    if (parseGenReg(p, regno)) {
        op.mode = M_RB;
        op.regno = regno;
        scan = p;
        return OK;
    }
    const auto sreg = parseSpReg(p, parser(), cpuType());
    if (sreg != REG_UNDEF) {
        op.mode = M_SA;
        op.sreg = sreg;
        scan = p;
        return OK;
    }
    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IM16;
    scan = p;
    return OK;
}

// A jump or call names the target address itself; the assembler works out the
// displacement.  Program-Counter relative reaches further than the absolute
// form, so it is used whenever the target is in range.  That choice is made
// from the distance alone, so an absolute encoding whose target a relative one
// also reaches comes back as the relative form through disassembly and
// reassembly: the target survives, the encoding does not.
void AsmAm29000::encodeTarget(AsmInsn &insn, const Operand &op) const {
    constexpr auto ABSOLUTE_MAX = UINT32_C(0x0003FFFF);
    const auto base = insn.address();
    const auto target = op.getError() ? base : op.val.getUnsigned();
    if (target & 3) {
        insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
        return;
    }
    const auto delta = branchDelta(base, target, insn, op) / 4;
    if (!overflowDelta(delta, 16)) {
        insn.embed((static_cast<uint32_t>(delta) & 0xFF00) << 8);
        insn.embed(static_cast<uint32_t>(delta) & 0xFF);
        return;
    }
    if (target > ABSOLUTE_MAX) {
        insn.setErrorIf(op, OPERAND_TOO_FAR);
        return;
    }
    const auto word = target / 4;
    insn.embed(BIT_A);
    insn.embed((word & 0xFF00) << 8);
    insn.embed(word & 0xFF);
}

void AsmAm29000::encodeControl(AsmInsn &insn, const Operand &op, uint_fast8_t pos, uint32_t max,
        AddrMode mode) const {
    const auto val = op.val.getUnsigned();
    if (op.mode != M_NONE) {
        if (val > max) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
        } else if (!controlValueOk(mode, static_cast<uint8_t>(val))) {
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        }
    }
    insn.embed((val & max) << pos);
}

void AsmAm29000::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    switch (mode) {
    case M_NONE:
        return;
    case M_RC:
        insn.embed(static_cast<uint32_t>(op.regno) << 16);
        return;
    case M_RA:
        insn.embed(static_cast<uint32_t>(op.regno) << 8);
        return;
    case M_RB:
        insn.embed(op.regno);
        return;
    case M_RBI:
        // A register keeps the M bit clear; a constant raises it.
        if (op.mode == M_RB) {
            insn.embed(op.regno);
        } else {
            if (op.val.overflowUint8())
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(BIT_M);
            insn.embed(op.val.getUnsigned() & 0xFF);
        }
        return;
    case M_IMH:
    case M_IM16: {
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        const auto val = op.val.getUnsigned();
        insn.embed((val & 0xFF00) << 8);
        insn.embed(val & 0xFF);
        return;
    }
    case M_TGT:
        encodeTarget(insn, op);
        return;
    case M_VN:
        encodeControl(insn, op, 16, 0xFF);
        return;
    case M_SA:
        if (op.mode == M_SA) {
            insn.embed(static_cast<uint32_t>(encodeSpReg(op.sreg)) << 8);
        } else {
            // The manual spells a register the same way whether it is named or
            // not, so a number stands in for a name.  It has to name a register
            // this CPU implements all the same, or the disassembler would not
            // read back what was written.
            const auto number = op.val.getUnsigned();
            if (op.val.overflowUint8()) {
                insn.setErrorIf(op, OVERFLOW_RANGE);
            } else if (decodeSpReg(number, cpuType()) == REG_UNDEF) {
                insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
            }
            insn.embed((number & 0xFF) << 8);
        }
        return;
    case M_CE:
        // Without a coprocessor interface the bit is reserved and the operand
        // may only be zero.
        encodeControl(insn, op, 23, hasCopro() ? 1 : 0);
        return;
    case M_CNTL:
        encodeControl(insn, op, 16, 0x7F);
        return;
    case M_ID:
        encodeControl(insn, op, 16, 3);
        return;
    case M_UI:
        encodeControl(insn, op, 7, 1);
        return;
    case M_RND:
        encodeControl(insn, op, 4, 7, mode);
        return;
    case M_FD:
    case M_TFMT:
        encodeControl(insn, op, 2, 3, mode);
        return;
    case M_FS:
    case M_FMT:
    case M_ACN:
        encodeControl(insn, op, 0, 3, mode);
        return;
    case M_ACNH:
        encodeControl(insn, op, 16, 3);
        return;
    case M_FUNC:
        encodeControl(insn, op, 18, 0xF);
        return;
    }
}

Error AsmAm29000::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
        auto &op = insn.operands[pos];
        if (parseOperand(scan, op) && op.hasError())
            return _insn.setError(op);
        if (op.mode == M_NONE)
            break;
        if (!scan.skipSpaces().expect(','))
            break;
    }
    scan.skipSpaces();

    if (searchName(cpuType(), hasFpu(), hasIntMul(), insn))
        return _insn.setError(insn.operands[0], insn);

    for (uint_fast8_t pos = 0; pos < MAX_OPERANDS; pos++) {
        const auto &op = insn.operands[pos];
        encodeOperand(insn, op, insn.mode(pos));
        // Carry over an error the operand tolerated while parsing, such as a
        // reference which stayed undefined.
        insn.setErrorIf(op);
    }
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace am29000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
